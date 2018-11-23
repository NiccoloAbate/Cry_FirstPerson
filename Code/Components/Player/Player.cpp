#include "StdAfx.h"
#include "Player.h"

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryEntitySystem\IEntity.h>

#include "GamePlugin.h"
#include "Game\CameraController.h"

#include "PlayerExtension.h"

static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent)

void CPlayerComponent::Initialize()
{
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	/*
	m_pInputComponent->RegisterAction("Player", "MouseX", [this](int activationMode, float value) { m_mouseDeltaRotation.x -= value; });
	m_pInputComponent->BindAction("Player", "MouseX", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

	m_pInputComponent->RegisterAction("Player", "MouseY", [this](int activationMode, float value) { m_mouseDeltaRotation.y -= value; });
	m_pInputComponent->BindAction("Player", "MouseY", eAID_KeyboardMouse, EKeyId::eKI_MouseY);
	*/

	REGISTER_PLAYEREXTESION_KEYEVENT("W", EKeyId::eKI_W);
	REGISTER_PLAYEREXTESION_KEYEVENT("A", EKeyId::eKI_A);
	REGISTER_PLAYEREXTESION_KEYEVENT("S", EKeyId::eKI_S);
	REGISTER_PLAYEREXTESION_KEYEVENT("D", EKeyId::eKI_D);

	REGISTER_PLAYEREXTESION_KEYEVENT("LShift", EKeyId::eKI_LShift);
	REGISTER_PLAYEREXTESION_KEYEVENT("Space", EKeyId::eKI_Space);

	REGISTER_PLAYEREXTESION_KEYEVENT("MouseX", EKeyId::eKI_MouseX);
	REGISTER_PLAYEREXTESION_KEYEVENT("MosueY", EKeyId::eKI_MouseY);
	
	m_pCamera = CGamePlugin::gGamePlugin->m_pCameraController->m_pCamera;
}

void CPlayerComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		if (m_pPlayerExtension)
			m_pPlayerExtension->Update(pCtx->fFrameTime);
	}
	break;
	default:
		break;
	}
}

void CPlayerComponent::UpdateLookDirectionRequest(float frameTime)
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
