#include "StdAfx.h"
#include "EnvironmentController.h"

#include "Components\Environment\Rain.h"
#include "Utils\ConsoleCommands.h"

// Console Commands
void CC_SetLevelWindSpeed(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(2)
	Vec3 WindSpeed;
	FromString(WindSpeed.x, pArgs->GetArg(1));
	FromString(WindSpeed.y, pArgs->GetArg(2));
	WindSpeed.z = 0;
	gEnv->p3DEngine->SetWind(WindSpeed);
}
ADDCONSOLECOMMAND("WindSpeed", CC_SetLevelWindSpeed)
//~Console Commands

void CEnvironmentController::Initialize()
{

	IEntity *pEnvironmentEntity = gEnv->pEntitySystem->FindEntityByName("EnvironmentProbe 01");
	if (pEnvironmentEntity)
		m_pProbe = pEnvironmentEntity->GetComponent<Cry::DefaultComponents::CEnvironmentProbeComponent>();
	//else
		//return;

	IEntity *pRainEntity = gEnv->pEntitySystem->FindEntityByName("g_Rain");
	if (pRainEntity)
		m_pRain = pRainEntity->GetComponent<CRainComponent>();
	else
	{
		//spawn rain component entity
	}

}

void CEnvironmentController::SetDiffuseMultiplier(float fMultiplier)
{
		m_pProbe->GetColorParameters().m_diffuseMultiplier = fMultiplier;
		SEntityEvent Event;
		Event.event = ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED;
		m_pProbe->SendEvent(Event);
}
