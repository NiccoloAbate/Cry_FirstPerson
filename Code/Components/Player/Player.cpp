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

	REGISTER_PLAYEREXTESION_KEYEVENT("W", EKeyId::eKI_W);
	REGISTER_PLAYEREXTESION_KEYEVENT("A", EKeyId::eKI_A);
	REGISTER_PLAYEREXTESION_KEYEVENT("S", EKeyId::eKI_S);
	REGISTER_PLAYEREXTESION_KEYEVENT("D", EKeyId::eKI_D);

	REGISTER_PLAYEREXTESION_KEYEVENT("LShift", EKeyId::eKI_LShift);
	REGISTER_PLAYEREXTESION_KEYEVENT("LCtrl", EKeyId::eKI_LCtrl);
	REGISTER_PLAYEREXTESION_KEYEVENT("LAlt", EKeyId::eKI_LAlt);
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

void CPlayerComponent::Release()
{
	m_pPlayerExtension->Release();
	m_pPlayerExtension = nullptr;
}

void CPlayerComponent::ExtendTo(CPlayerExtension * pPlayerExtension)
{
	m_pPlayerExtension = pPlayerExtension;
	m_pPlayerExtension->ExtendFrom(this);
}

