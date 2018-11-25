#include "StdAfx.h"
#include "CharacterPlayerExtension.h"

#include "Components\Player\Player.h"

#include "GamePlugin.h"
#include "Timeline\TimelineManager.h"

#include "Character.h"
#include "Components\Game\Stats.h"
#include "UI\Hud.h"


void CCharacter_PlayerExtension::Initialize()
{
}

void CCharacter_PlayerExtension::Update(float fFrameTime)
{
	UpdateMovementRequest(fFrameTime);
	UpdateLookDirectionRequest(fFrameTime);
	UpdateAnimation(fFrameTime);
	UpdateCamera(fFrameTime);
}

void CCharacter_PlayerExtension::ProcessKeyEvent(EKeyId KeyId, int activationMode, float value)
{
	switch (KeyId)
	{
		// Movement
	case eKI_W:
		HandleInputFlagChange((TInputFlags)EInputFlag::MoveForward, activationMode);
		break;
	case eKI_A:
		HandleInputFlagChange((TInputFlags)EInputFlag::MoveLeft, activationMode);
		break;
	case eKI_S:
		HandleInputFlagChange((TInputFlags)EInputFlag::MoveBack, activationMode);
		break;
	case eKI_D:
		HandleInputFlagChange((TInputFlags)EInputFlag::MoveRight, activationMode);
		break;


		// Orientation
	case eKI_MouseX:
		m_pCharacterComponent->m_mouseDeltaRotation.x -= value;
		break;
	case eKI_MouseY:
		m_pCharacterComponent->m_mouseDeltaRotation.y -= value;
		break;
		// Key Toggles
	case eKI_LShift:
		HandleInputFlagChange((TInputFlags)EInputFlag::Sprint, activationMode);
		break;
	case eKI_LCtrl:
		HandleInputFlagChange((TInputFlags)EInputFlag::LCTRLDOWN, activationMode);
		break;
	case eKI_LAlt:
		HandleInputFlagChange((TInputFlags)EInputFlag::ALTDOWN, activationMode);
		break;


		// Actions
	case eKI_Space:
		Jump();
		break;
	default:
		break;
	}
}

void CCharacter_PlayerExtension::UpdateMovementRequest(float frameTime)
{
	// Don't handle input if we are in air
	//if (!m_pCharacterController->IsOnGround())
	//return;

	Stat<float> &Stamina = m_pCharacterComponent->m_pStatsComponent->GetStamina();
	// Don't calculate movement if stamina is out
	if (Stamina <= 0)
		return;

	Vec3 velocity = ZERO;


	float SpeedMultiplyer = 1;
	if (m_InputFlags.HasFlag((TInputFlags)EInputFlag::Sprint))
		SpeedMultiplyer = 1.5;
	if (!m_pCharacterComponent->m_pCharacterController->IsOnGround())
		SpeedMultiplyer = .15;

	const float moveSpeed = 20.5f * SpeedMultiplyer;

	static bool bHasBeenMoving = false;
	if (m_InputFlags.HasFlag((TInputFlags)EInputFlag::MoveLeft))
	{
		velocity.x -= 1;
	}
	if (m_InputFlags.HasFlag((TInputFlags)EInputFlag::MoveRight))
	{
		velocity.x += 1;
	}
	if (m_InputFlags.HasFlag((TInputFlags)EInputFlag::MoveForward))
	{
		velocity.y += 1;
	}
	if (m_InputFlags.HasFlag((TInputFlags)EInputFlag::MoveBack))
	{
		velocity.y -= 1;
	}

	// Calculate Stamina drain
	bool bMoving = !velocity.IsZero();
	if (bMoving)
	{
		velocity.normalize();
		velocity *= moveSpeed * frameTime;

		if (SpeedMultiplyer < 1)
			SpeedMultiplyer = 1;
		Stamina -= frameTime * 10 * SpeedMultiplyer;
	}
	else
	{
		//Stamina += frameTime * 5;
	}
	GetHud()->UpdateStaminaBar(); // TODO

	if (bMoving != bHasBeenMoving)
	{
		bHasBeenMoving = bMoving;
		CTimelineManager *pTimeLineManager = CGamePlugin::gGamePlugin->m_pTimelineManager;
		if (bMoving)
			pTimeLineManager->SetTimeScale(1);
		else
			pTimeLineManager->SetTimeScale(0);
	}

	m_pCharacterComponent->m_pCharacterController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
}

void CCharacter_PlayerExtension::UpdateLookDirectionRequest(float frameTime)
{
	const float rotationSpeed = 0.002f;
	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	Vec2 &mouseDeltaRotation = m_pCharacterComponent->m_mouseDeltaRotation;
	MovingAverage<Vec2, 10> &mouseDeltaSmoothingFilter = m_pCharacterComponent->m_mouseDeltaSmoothingFilter;

	Quat &lookOrientation = m_pCharacterComponent->m_lookOrientation; //!< Should translate to head orientation in the future
	float &horizontalAngularVelocity = m_pCharacterComponent->m_horizontalAngularVelocity;
	MovingAverage<float, 10> &averagedHorizontalAngularVelocity = m_pCharacterComponent->m_averagedHorizontalAngularVelocity;

	if (mouseDeltaRotation.IsEquivalent(ZERO, MOUSE_DELTA_TRESHOLD))
		return;

	// Apply smoothing filter to the mouse input
	mouseDeltaRotation = mouseDeltaSmoothingFilter.Push(mouseDeltaRotation).Get();

	// Update angular velocity metrics
	horizontalAngularVelocity = (mouseDeltaRotation.x * rotationSpeed) / frameTime;
	averagedHorizontalAngularVelocity.Push(horizontalAngularVelocity);

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(lookOrientation));

	// Yaw
	ypr.x += mouseDeltaRotation.x * rotationSpeed;

	// Pitch
	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + mouseDeltaRotation.y * rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);

	// Roll (skip)
	ypr.z = 0;

	lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset the mouse delta accumulator every frame
	mouseDeltaRotation = ZERO;
}

void CCharacter_PlayerExtension::UpdateAnimation(float frameTime)
{
	const float angularVelocityTurningThreshold = 0.174; // [rad/s]

	Quat &lookOrientation = m_pCharacterComponent->m_lookOrientation;
	const float horizontalAngularVelocity = m_pCharacterComponent->m_horizontalAngularVelocity;
	MovingAverage<float, 10> &averagedHorizontalAngularVelocity = m_pCharacterComponent->m_averagedHorizontalAngularVelocity;

	const FragmentID idleFragmentId = m_pCharacterComponent->m_idleFragmentId;
	const FragmentID walkFragmentId = m_pCharacterComponent->m_walkFragmentId;
	const TagID rotateTagId = m_pCharacterComponent->m_rotateTagId;

	FragmentID &activeFragmentId = m_pCharacterComponent->m_activeFragmentId;

														 // Update tags and motion parameters used for turning
	const bool isTurning = std::abs(averagedHorizontalAngularVelocity.Get()) > angularVelocityTurningThreshold;
	m_pCharacterComponent->m_pAnimationComponent->SetTagWithId(rotateTagId, isTurning);
	if (isTurning)
	{
		// TODO: This is a very rough predictive estimation of eMotionParamID_TurnAngle that could easily be replaced with accurate reactive motion 
		// if we introduced IK look/aim setup to the character's model and decoupled entity's orientation from the look direction derived from mouse input.

		const float turnDuration = 1.0f; // Expect the turning motion to take approximately one second.
		m_pCharacterComponent->m_pAnimationComponent->SetMotionParameter(eMotionParamID_TurnAngle, horizontalAngularVelocity * turnDuration);
	}

	// Update active fragment
	const auto& desiredFragmentId = m_pCharacterComponent->m_pCharacterController->IsWalking() ? walkFragmentId : idleFragmentId;
	if (activeFragmentId != desiredFragmentId)
	{
		activeFragmentId = desiredFragmentId;
		m_pCharacterComponent->m_pAnimationComponent->QueueFragmentWithId(activeFragmentId);
	}

	// Update entity rotation as the player turns
	// We only want to affect Z-axis rotation, zero pitch and roll
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(lookOrientation));
	ypr.y = 0;
	ypr.z = 0;
	const Quat correctedOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Send updated transform to the entity, only orientation changes
	GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), correctedOrientation, Vec3(1, 1, 1));
}

void CCharacter_PlayerExtension::UpdateCamera(float frameTime)
{
	if (m_pPlayerComponent->GetCameraEntity()->GetParent() != m_pEntity)
		return;

	Vec2 &mouseDeltaRotation = m_pCharacterComponent->m_mouseDeltaRotation;
	
	Quat &lookOrientation = m_pCharacterComponent->m_lookOrientation;

	const float rotationSpeed = m_pCharacterComponent->m_rotationSpeed;
	const int cameraJointId = m_pCharacterComponent->m_cameraJointId;

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(lookOrientation));

	ypr.x += mouseDeltaRotation.x * rotationSpeed;

	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + mouseDeltaRotation.y * rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);
	// Skip roll
	ypr.z = 0;

	lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset every frame
	mouseDeltaRotation = ZERO;

	// Ignore z-axis rotation, that's set by CPlayerAnimations
	ypr.x = 0;

	// Start with changing view rotation to the requested mouse look orientation
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

	const float viewOffsetForward = 0.01f;
	const float viewOffsetUp = 0.26f;

	if (ICharacterInstance *pCharacter = m_pCharacterComponent->m_pAnimationComponent->GetCharacter())
	{
		// Get the local space orientation of the camera joint
		const QuatT &cameraOrientation = pCharacter->GetISkeletonPose()->GetAbsJointByID(cameraJointId);
		// Apply the offset to the camera
		localTransform.SetTranslation(cameraOrientation.t + Vec3(0, viewOffsetForward, viewOffsetUp));
	}

	//m_pCameraComponent->SetTransformMatrix(localTransform);
	//m_pCamera->SetLocalTM(localTransform);
	m_pPlayerComponent->GetCameraEntity()->SetLocalTM(localTransform);
}

void CCharacter_PlayerExtension::Jump()
{
	if (!m_pCharacterComponent->m_pCharacterController->IsOnGround())
		return;
	Stat<float> &Stamina = m_pCharacterComponent->m_pStatsComponent->GetStamina();
	if (Stamina <= 0)
		return;

	if (IPhysicalEntity* pPhysicalEntity = m_pCharacterComponent->m_pCharacterController->GetEntity()->GetPhysicalEntity())
	{
		const float JumpHeight = 500;

		pe_action_impulse Jump;
		Jump.impulse = Vec3(0, 0, 1) * JumpHeight;

		pPhysicalEntity->Action(&Jump);
	}
}
