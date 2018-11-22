#include "StdAfx.h"
#include "CryFirstPersonPlayer.h"

#include "Components/Bullet.h"
#include "Components/SpawnPoint.h"

#include <CryRenderer/IRenderAuxGeom.h>

#include <IActionMapManager.h>

#define MOUSE_DELTA_TRESHOLD 0.0001f

#include "GamePlugin.h"
#include <CrySystem\ISystem.h>

#include "Components\Hive\Pheromone.h"
#include "Components\Hive\HiveAgent.h"
#include "Components\Characters\Character.h"

#include "Game\CameraController.h"
#include "Dialogue\DialogueManager.h"
#include "Components\Game\Stats.h"

#include "GamePlugin.h"
#include "Timeline\TimelineManager.h"
#include "Game\UIController.h"
#include "UI\Hud.h"

#include "Utils\Utils.h"

void CCryFirstPersonPlayerComponent::Initialize()
{

	// Create the camera component, will automatically update the viewport every frame
	//m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();

	// The character controller is responsible for maintaining player physics
	m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	// Offset the default character controller up by one unit
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	// Create the advanced animation component, responsible for updating Mannequin and animating the player
	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

	// Set the player geometry, this also triggers physics proxy creation
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/SampleCharacter/firstperson.cdf");

	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("FirstPersonCharacter");
	// Queue the idle fragment to start playing immediately on next update
	m_pAnimationComponent->SetDefaultFragmentName("Idle");

	// Disable movement coming from the animation (root joint offset), we control this entirely via physics
	m_pAnimationComponent->SetAnimationDrivenMotion(false);

	// Load the character and Mannequin data from file
	m_pAnimationComponent->LoadFromDisk();

	// Acquire fragment and tag identifiers to avoid doing so each update
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");

	// Get the input component, wraps access to action mapping so we can easily get callbacks when inputs are triggered
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	// Register an action, and the callback that will be sent when it's triggered
	m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveLeft, activationMode);  });
	// Bind the 'A' key the "moveleft" action
	m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A);

	m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveRight, activationMode);  });
	m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);

	m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveForward, activationMode);  });
	m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

	m_pInputComponent->RegisterAction("player", "moveback", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveBack, activationMode);  });
	m_pInputComponent->BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

	m_pInputComponent->RegisterAction("player", "sprint", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::Sprint, activationMode); });
	m_pInputComponent->BindAction("player", "sprint", eAID_KeyboardMouse, EKeyId::eKI_LShift);
	
	m_pInputComponent->RegisterAction("player", "jump", [this](int activationMode, float value) { ACTIVECHECK Jump(); });
	m_pInputComponent->BindAction("player", "jump", eAID_KeyboardMouse, EKeyId::eKI_Space, true, false, false);

	m_pInputComponent->RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) { ACTIVECHECK m_mouseDeltaRotation.x -= value; });
	m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

	m_pInputComponent->RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) { ACTIVECHECK m_mouseDeltaRotation.y -= value; });
	m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);

	// Register the shoot action
	m_pInputComponent->RegisterAction("player", "shoot", [this](int activationMode, float value)
	{
		ACTIVECHECK
		// Only fire on press, not release
		if (activationMode == eIS_Pressed)
		{
			if (ICharacterInstance *pCharacter = m_pAnimationComponent->GetCharacter())
			{
				auto *pBarrelOutAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("barrel_out");

				if (pBarrelOutAttachment != nullptr)
				{
					QuatTS bulletOrigin = pBarrelOutAttachment->GetAttWorldAbsolute();

					SEntitySpawnParams spawnParams;
					spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();

					spawnParams.vPosition = bulletOrigin.t;
					spawnParams.qRotation = bulletOrigin.q;

					const float bulletScale = 0.05f;
					spawnParams.vScale = Vec3(bulletScale);

					// Spawn the entity
					if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
					{
						// See Bullet.cpp, bullet is propelled in  the rotation and position the entity was spawned with
						pEntity->CreateComponentClass<CBulletComponent>();
					}
				}
			}
		}
	});
	m_pInputComponent->BindAction("player", "shoot", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

	m_pInputComponent->RegisterAction("player", "log", [this](int activationMode, float value) 
	{
		ACTIVECHECK
		//if (activationMode == eIS_Pressed)
			gEnv->pLog->Log("Worked");
	});
	m_pInputComponent->BindAction("player", "log", eAID_KeyboardMouse, eKI_L, true, false, false);

	m_pInputComponent->RegisterAction("game", "quit", [this](int activationMode, float value) { gEnv->pConsole->ExecuteString("quit"); });
	m_pInputComponent->BindAction("game", "quit", eAID_KeyboardMouse, eKI_Escape, false, true, false);

	m_pInputComponent->RegisterAction("game", "switchplayer", [this](int activationMode, float value) { ACTIVECHECK CGamePlugin::gGamePlugin->SwitchPlayers(); });
	m_pInputComponent->BindAction("game", "switchplayer", eAID_KeyboardMouse, eKI_Tab, false, true, false);

	m_pInputComponent->RegisterAction("game", "testaction", [this](int activationMode, float value)
	{ 
		ACTIVECHECK
		IEntity *pEntity = gEnv->pEntitySystem->FindEntityByName("Agent 1");
		CHiveAgentComponent *pAgent1 = pEntity->GetComponent<CHiveAgentComponent>();
		pAgent1->GoHome();
	});
	m_pInputComponent->BindAction("game", "testaction", eAID_KeyboardMouse, eKI_G, false, true, false);

	m_pInputComponent->RegisterAction("input", "toggleshift", [this](int activationMode, float value) {  HandleInputFlagChange((TInputFlags)EInputFlag::SHIFTDOWN, activationMode); });
	m_pInputComponent->BindAction("input", "toggleshift", eAID_KeyboardMouse, eKI_LShift, true, true, false);

	m_pInputComponent->RegisterAction("input", "togglecontrol", [this](int activationMode, float value) {  HandleInputFlagChange((TInputFlags)EInputFlag::CONTROLDOWN, activationMode); });
	m_pInputComponent->BindAction("input", "togglecontrol", eAID_KeyboardMouse, eKI_LCtrl, true, true, false);

	m_pInputComponent->RegisterAction("input", "togglealt", [this](int activationMode, float value) {  HandleInputFlagChange((TInputFlags)EInputFlag::ALTDOWN, activationMode); });
	m_pInputComponent->BindAction("input", "togglealt", eAID_KeyboardMouse, eKI_LAlt, true, true, false);

	m_pInputComponent->RegisterAction("game", "overheadcamera", [this](int activationMode, float value) {ACTIVECHECK INPUTFLAGCHECK_NOT(ALTDOWN) CGamePlugin::gGamePlugin->m_pCameraController->OverheadCameraView(); });
	m_pInputComponent->BindAction("game", "overheadcamera", eAID_KeyboardMouse, eKI_Up, false, true, false);

	m_pInputComponent->RegisterAction("game", "talk", [this](int activationMode, float value) 
	{
		ACTIVECHECK
		if (!m_pLookEntity)
			return;
		if (CGamePlugin::gGamePlugin->m_pDialogueManager->IsInDialogue())
			return;
		if (CCharacterComponent *pCharacter = m_pLookEntity->GetComponent<CCharacterComponent>())
		{
			CDialogueManager *pDialogueManager = CGamePlugin::gGamePlugin->m_pDialogueManager;
			pDialogueManager->LoadDialogue("Characters/Dialogues/TestDialogue");
			pDialogueManager->m_pCurrentDialogue->m_pCharacter = pCharacter;
			pDialogueManager->m_pCurrentDialogue->Start();
		}
	});
	m_pInputComponent->BindAction("game", "talk", eAID_KeyboardMouse, eKI_Mouse1, false, true, false);



	m_pStatsComponent = m_pEntity->GetOrCreateComponent<CStatsComponent>();
	m_pStatsComponent->GiveStamina({ 0, 100, 100 });
	m_pStatsComponent->GiveHealth({ 0, 100, 100 });

	Revive();
}

uint64 CCryFirstPersonPlayerComponent::GetEventMask() const
{
	return BIT64(ENTITY_EVENT_START_GAME) | BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_COLLISION);
}

void CCryFirstPersonPlayerComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_START_GAME:
	{
		// Revive the entity when gameplay starts
		Revive();
	}
	break;
	case ENTITY_EVENT_COLLISION:
	{
		// Collision info can be retrieved using the event pointer
		EventPhysCollision *physCollision = reinterpret_cast<EventPhysCollision *>(event.nParam[0]);
		physCollision->vloc[0];
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		ACTIVECHECK

		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		// Start by updating the movement request we want to send to the character controller
		// This results in the physical representation of the character moving
		UpdateMovementRequest(pCtx->fFrameTime);

		//Update other toggle KeyBind action
		UpdateKeyBindRequests(pCtx->fFrameTime);

		// Process mouse input to update look orientation.
		UpdateLookDirectionRequest(pCtx->fFrameTime);

		// Update the animation state of the character
		UpdateAnimation(pCtx->fFrameTime);

		// Update the camera component offset
		UpdateCamera(pCtx->fFrameTime);

		Update(pCtx->fFrameTime);
	}
	break;
	}
}

void CCryFirstPersonPlayerComponent::UpdateMovementRequest(float frameTime)
{
	// Don't handle input if we are in air
	//if (!m_pCharacterController->IsOnGround())
		//return;

	Stat<float> &Stamina = m_pStatsComponent->GetStamina();
	// Don't calculate movement if stamina is out
	if (Stamina <= 0)
		return;

	Vec3 velocity = ZERO;

	
	float SpeedMultiplyer = 1;
	if (m_inputFlags & (TInputFlags)EInputFlag::Sprint)
		SpeedMultiplyer = 1.5;
	if (!m_pCharacterController->IsOnGround())
		SpeedMultiplyer = .15;

	const float moveSpeed = 20.5f * SpeedMultiplyer;

	static bool bHasBeenMoving = false;
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveLeft)
	{
		velocity.x -= 1;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveRight)
	{
		velocity.x += 1;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveForward)
	{
		velocity.y += 1;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveBack)
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
	GetHud()->UpdateStaminaBar();

	if (bMoving != bHasBeenMoving)
	{
		bHasBeenMoving = bMoving;
		CTimelineManager *pTimeLineManager = CGamePlugin::gGamePlugin->m_pTimelineManager;
		if (bMoving)
			pTimeLineManager->SetTimeScale(1);
		else
			pTimeLineManager->SetTimeScale(0);
	}

	m_pCharacterController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
}

void CCryFirstPersonPlayerComponent::UpdateKeyBindRequests(float frameTime)
{

}

void CCryFirstPersonPlayerComponent::UpdateLookDirectionRequest(float frameTime)
{
	const float rotationSpeed = 0.002f;
	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	if (m_mouseDeltaRotation.IsEquivalent(ZERO, MOUSE_DELTA_TRESHOLD))
		return;

	// Apply smoothing filter to the mouse input
	m_mouseDeltaRotation = m_mouseDeltaSmoothingFilter.Push(m_mouseDeltaRotation).Get();

	// Update angular velocity metrics
	m_horizontalAngularVelocity = (m_mouseDeltaRotation.x * rotationSpeed) / frameTime;
	m_averagedHorizontalAngularVelocity.Push(m_horizontalAngularVelocity);

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	// Yaw
	ypr.x += m_mouseDeltaRotation.x * rotationSpeed;

	// Pitch
	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);

	// Roll (skip)
	ypr.z = 0;

	m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset the mouse delta accumulator every frame
	m_mouseDeltaRotation = ZERO;
}

void CCryFirstPersonPlayerComponent::UpdateAnimation(float frameTime)
{
	const float angularVelocityTurningThreshold = 0.174; // [rad/s]

	// Update tags and motion parameters used for turning
	const bool isTurning = std::abs(m_averagedHorizontalAngularVelocity.Get()) > angularVelocityTurningThreshold;
	m_pAnimationComponent->SetTagWithId(m_rotateTagId, isTurning);
	if (isTurning)
	{
		// TODO: This is a very rough predictive estimation of eMotionParamID_TurnAngle that could easily be replaced with accurate reactive motion 
		// if we introduced IK look/aim setup to the character's model and decoupled entity's orientation from the look direction derived from mouse input.

		const float turnDuration = 1.0f; // Expect the turning motion to take approximately one second.
		m_pAnimationComponent->SetMotionParameter(eMotionParamID_TurnAngle, m_horizontalAngularVelocity * turnDuration);
	}

	// Update active fragment
	const auto& desiredFragmentId = m_pCharacterController->IsWalking() ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId)
	{
		m_activeFragmentId = desiredFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}

	// Update entity rotation as the player turns
	// We only want to affect Z-axis rotation, zero pitch and roll
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));
	ypr.y = 0;
	ypr.z = 0;
	const Quat correctedOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Send updated transform to the entity, only orientation changes
	GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), correctedOrientation, Vec3(1, 1, 1));
}

void CCryFirstPersonPlayerComponent::UpdateCamera(float frameTime)
{

	if (m_pCamera->GetParent() != m_pEntity)
		return;

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	ypr.x += m_mouseDeltaRotation.x * m_rotationSpeed;

	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * m_rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);
	// Skip roll
	ypr.z = 0;

	m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset every frame
	m_mouseDeltaRotation = ZERO;

	// Ignore z-axis rotation, that's set by CPlayerAnimations
	ypr.x = 0;

	// Start with changing view rotation to the requested mouse look orientation
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

	const float viewOffsetForward = 0.01f;
	const float viewOffsetUp = 0.26f;

	if (ICharacterInstance *pCharacter = m_pAnimationComponent->GetCharacter())
	{
		// Get the local space orientation of the camera joint
		const QuatT &cameraOrientation = pCharacter->GetISkeletonPose()->GetAbsJointByID(m_cameraJointId);
		// Apply the offset to the camera
		localTransform.SetTranslation(cameraOrientation.t + Vec3(0, viewOffsetForward, viewOffsetUp));
	}

	//m_pCameraComponent->SetTransformMatrix(localTransform);
	m_pCamera->SetLocalTM(localTransform);
}

void CCryFirstPersonPlayerComponent::Update(float frameTime)
{
	UpdateLookEntity(frameTime);
}

void CCryFirstPersonPlayerComponent::Revive()
{
	// Find a spawn point and move the entity there
	SpawnAtSpawnPoint();

	// Unhide the entity in case hidden by the Editor
	GetEntity()->Hide(false);

	// Make sure that the player spawns upright
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	// Apply the character to the entity and queue animations
	m_pAnimationComponent->ResetCharacter();
	m_pCharacterController->Physicalize();

	// Reset input now that the player respawned
	m_inputFlags = 0;
	m_mouseDeltaRotation = ZERO;
	m_lookOrientation = IDENTITY;

	m_mouseDeltaSmoothingFilter.Reset();

	m_activeFragmentId = FRAGMENT_ID_INVALID;

	m_horizontalAngularVelocity = 0.0f;
	m_averagedHorizontalAngularVelocity.Reset();

	if (ICharacterInstance *pCharacter = m_pAnimationComponent->GetCharacter())
	{
		// Cache the camera joint id so that we don't need to look it up every frame in UpdateView
		m_cameraJointId = pCharacter->GetIDefaultSkeleton().GetJointIDByName("head");
	}
}

void CCryFirstPersonPlayerComponent::SetActiveChar()
{
	//m_pEntity->AddComponent(std::shared_ptr<IEntityComponent>(m_pCameraComponent));
	m_pEntity->AttachChild(m_pCamera);
	SetActive(true);
}

void CCryFirstPersonPlayerComponent::SetInactiveChar()
{
	//m_pEntity->RemoveComponent(m_pCameraComponent);
	SetActive(false);
}

void CCryFirstPersonPlayerComponent::SetCamera(IEntity *pCamera)
{
	m_pCamera = pCamera;
}

void CCryFirstPersonPlayerComponent::SpawnAtSpawnPoint()
{
	// We only handle default spawning below for the Launcher
	// Editor has special logic in CEditorGame
	if (gEnv->IsEditor())
		return;

	// Spawn at first default spawner
	auto *pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
	pEntityIterator->MoveFirst();

	while (!pEntityIterator->IsEnd())
	{
		IEntity *pEntity = pEntityIterator->Next();

		if (auto* pSpawner = pEntity->GetComponent<CSpawnPointComponent>())
		{
			pSpawner->SpawnEntity(m_pEntity);
			break;
		}
	}
}

void CCryFirstPersonPlayerComponent::Jump(){

	if (!m_pCharacterController->IsOnGround())
		return;
	Stat<float> &Stamina = m_pStatsComponent->GetStamina();
	if (Stamina <= 0)
		return;

	if (IPhysicalEntity* pPhysicalEntity = m_pCharacterController->GetEntity()->GetPhysicalEntity()) {

		const float JumpHeight = 1000;

		pe_action_impulse Jump;
		Jump.impulse = Vec3(0, 0, 1) * JumpHeight;
		
		pPhysicalEntity->Action(&Jump);

	}
}

void CCryFirstPersonPlayerComponent::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type)
{
	switch (type)
	{
	case EInputFlagType::Hold:
	{
		if (activationMode == eIS_Released)
		{
			m_inputFlags &= ~flags;
		}
		else
		{
			m_inputFlags |= flags;
		}
	}
	break;
	case EInputFlagType::Toggle:
	{
		if (activationMode == eIS_Released)
		{
			// Toggle the bit(s)
			m_inputFlags ^= flags;
		}
	}
	break;
	}
}

void CCryFirstPersonPlayerComponent::UpdateLookEntity(float frameTime)
{
	static int _Time = 0;
	if (_Time != 6)
	{
		_Time++;
		return;
	}
	else
		_Time = 0;

	const Vec3 Org = m_pCamera->GetWorldPos();
	const Vec3 Dir = m_pCamera->GetForwardDir().scale(10);
	
	ray_hit Hits;
	const int nHits = gEnv->pPhysicalWorld->RayWorldIntersection(Org, Dir, ent_all, rwi_colltype_any | rwi_ignore_back_faces, &Hits, 1, m_pEntity->GetPhysicalEntity());

	//if (Hits.bTerrain) { if (!_Changed) return; RemoveLookEntityHighlight(); m_pLookEntity = nullptr; _Changed = false; return; }

	IEntity *pHitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(Hits.pCollider);
	if (!pHitEntity) 
	{
		RemoveLookEntityHighlight(); 
		m_pLookEntity = nullptr; 
		return; 
	}
	if(pHitEntity == m_pLookEntity)
		return;
	//Highlight new look entity
	Utils::EntitySilhouette(pHitEntity, 0x0000ffff);
	//Remove old highlight
	RemoveLookEntityHighlight();
	m_pLookEntity = pHitEntity;
}

void CCryFirstPersonPlayerComponent::RemoveLookEntityHighlight()
{
	if (!m_pLookEntity)
		return;
	Utils::EntitySilhouette(m_pLookEntity, 0x00000000);
}

CHud * CCryFirstPersonPlayerComponent::GetHud()
{
	return CGamePlugin::gGamePlugin->m_pUIController->m_pHud;
}
