#include <StdAfx.h>
#include "Rain.h"

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

#include "GamePlugin.h"
#include "Game\CameraController.h"
#include "Game\EnvironmentController.h"
#include "Utils\ConsoleCommands.h"
#include "Utils\Macros\Checks.h"

static void RegisterRainComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CRainComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterRainComponent)

// Console Commands
/*
void CC_SetWindSpeed(CC_Args pArgs)
{
	if (pArgs->GetArgCount() <= 2)
		return;
	Vec2 WindSpeed;
	FromString(WindSpeed.x, pArgs->GetArg(1));
	FromString(WindSpeed.y, pArgs->GetArg(2));
	CGamePlugin::gGamePlugin->m_pEnvironmentController->m_pRain->SetWindSpeed(WindSpeed);
}
ADDCONSOLECOMMAND("Rain_WindSpeed", CC_SetWindSpeed)
void CC_UseLevelWind(CC_Args pArgs)
{
	if (pArgs->GetArgCount() <= 1)
		return;
	bool bUseLevelWind;
	FromString(bUseLevelWind, pArgs->GetArg(1));
	CGamePlugin::gGamePlugin->m_pEnvironmentController->m_pRain->UseLevelWind(bUseLevelWind);
}
ADDCONSOLECOMMAND("Rain_UseLevelWind", CC_UseLevelWind)
*/
void CC_RainOn(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1)
	CC_GETARG(bool, bRainOn, 1)
	CGamePlugin::gGamePlugin->m_pEnvironmentController->m_pRain->Activate(bRainOn);
}
ADDCONSOLECOMMAND("Rain_On", CC_RainOn)
void CC_RainThickness(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1)
	CC_GETARG(float, RainThickness, 1)
	CGamePlugin::gGamePlugin->m_pEnvironmentController->m_pRain->SetThickness(RainThickness);
}
ADDCONSOLECOMMAND("Rain_Thickness", CC_RainThickness)
//~ConsoleCommands


void CRainComponent::Reset()
{
	if (!m_pEmitter)
		return;
	m_pEmitter->GetAttributes().SetAsFloat(m_pEmitter->GetAttributes().FindAttributeIdByName("WindSpeed_X"), m_RainParams.WindSpeed_X);
	m_pEmitter->GetAttributes().SetAsFloat(m_pEmitter->GetAttributes().FindAttributeIdByName("WindSpeed_Y"), m_RainParams.WindSpeed_Y);
	m_pEmitter->GetAttributes().SetAsFloat(m_pEmitter->GetAttributes().FindAttributeIdByName("Thickness"), m_RainParams.Thickness);
	m_pParticleComponent->Activate(m_RainParams.bActive);
}

void CRainComponent::Initialize()
{
	m_pParticleComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CParticleComponent>();
	m_pParticleComponent->SetEffectName("ParticleEffects/Environment/Rain.pfx");
	//m_pParticleComponent->LoadEffect(true); //Fucks shit up
	//m_pParticleComponent->Activate(true);

	//m_RainParams.WindSpeed_X = -10; ALL LEVEL WIND NOW
	//m_RainParams.WindSpeed_Y = -3; ALL LEVEL WIND NOW

	//SChildAttachParams AttachParams;
	//AttachParams.m_nAttachFlags = ENTITY_XFORM_POS;

}

void CRainComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
		Reset();
		break;
	case ENTITY_EVENT_LEVEL_LOADED:
		m_pEmitter = m_pParticleComponent->GetEntity()->GetParticleEmitter(m_pParticleComponent->GetEntitySlotId());
		Reset();
		break;
	case ENTITY_EVENT_UPDATE:
	{
		const Vec3 CamPos = CGamePlugin::gGamePlugin->m_pCameraController->m_pCameraEntity->GetWorldPos();
		m_pEntity->SetPos(CamPos);

		DEBUG_CODE(	if(!gEnv->IsEditor() || gEnv->IsEditorGameMode())	)
		{
			const float Radius = 5 * m_RainParams.AreaSize; // 5 is half the actual size
			const Vec3 Min = Vec3(CamPos.x - Radius + m_RainParams.WindSpeed_X, CamPos.y - Radius + m_RainParams.WindSpeed_Y, CamPos.z - 10);
			const Vec3 Max = Vec3(CamPos.x + Radius + m_RainParams.WindSpeed_X, CamPos.y + Radius + m_RainParams.WindSpeed_Y, CamPos.z + 10);
			const AABB RainArea = AABB(Min, Max);
			const Vec3 WindSpeed = gEnv->p3DEngine->GetWind(RainArea, false);
			SetWindSpeed(WindSpeed);
		}
	}
		break;
	default:
		break;
	}
}
