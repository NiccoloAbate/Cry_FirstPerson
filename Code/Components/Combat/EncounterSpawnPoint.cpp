#include "StdAfx.h"
#include "EncounterSpawnPoint.h"

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

#include "Components/Player/Player.h"
#include "Components/Hive/HiveAgent.h"
#include "Gameflow/Combat/CombatGameflow.h"
#include "Gameflow/GameflowManager.h"
#include "GamePlugin.h"

static void RegisterEncounterSpawnPointComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CEncounterSpawnPoint));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterEncounterSpawnPointComponent)

void CEncounterSpawnPoint::Initialize()
{
	Reset();
}

void CEncounterSpawnPoint::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_EDITOR_PROPERTY_CHANGED:
		// Link sUnitType and iUnitType
		break;
	default:
		break;
	}
}

void CEncounterSpawnPoint::MoveTo(IEntity* pUnit)
{
	pUnit->SetWorldTM(m_pEntity->GetWorldTM());
}

void CEncounterSpawnPoint::MovePlayerTo()
{
	CGamePlugin::gGamePlugin->GetPlayerExtensionEntity()->SetWorldTM(m_pEntity->GetWorldTM());
	CGamePlugin::gGamePlugin->GetPlayerComponent()->SetViewDir(m_pEntity->GetForwardDir());
	//CGamePlugin::gGamePlugin->GetPlayerComponent()->FaceAt()
}

IEntity* CEncounterSpawnPoint::SpawnAt()
{

	SEntitySpawnParams SpawnParams;
	SpawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	SpawnParams.vPosition = m_pEntity->GetWorldPos();
	SpawnParams.qRotation = m_pEntity->GetWorldRotation();
	SpawnParams.sName = "TODO";
	IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(SpawnParams);

	if (!pEntity)
		return nullptr;

	switch (m_iUnitType)
	{
	case eUnitTypes::Minion:
		pEntity->CreateComponent<CHiveAgentComponent>();
		break;
	default:
		break;
	}

	CCombatGameflow* pCombatGameflow = (CCombatGameflow*)CGamePlugin::gGamePlugin->m_pGameflowManager->GetGameflowFromId(Gameflow::Gameflows::COMBAT);
	pCombatGameflow->RegisterEnemy(pEntity);

	return m_pConnectedEntity = pEntity;
}


#include "Combat/CombatSystem.h"
void CEncounterSpawnPoint::Reset()
{
	
	if (!gEnv->IsEditor() && !CCombatManager::GetDebugFlags().HasFlag(Combat::eDebugFlags::SHOW_SPAWNPOINTS))
		return;

	string GeomFilePath;
	switch (m_iUnitType)
	{
	case eUnitTypes::Minion:
		GeomFilePath = "Objects/Editor/Spawnpoint_Generic/SpawnPoint_Generic.cgf";
		break;
	default:
		GeomFilePath = "Objects/Editor/Spawnpoint_Generic/SpawnPoint_Generic.cgf";
		break;
	}

	if (GeomFilePath != m_GeomFilePath)
	{
		m_GeomFilePath = GeomFilePath;
		m_pEntity->LoadGeometry(0, GeomFilePath);
		IMaterial* const pDefaultMat = gEnv->p3DEngine->GetMaterialManager()->GetDefaultMaterial();
		m_pEntity->SetMaterial(pDefaultMat);
	}
}
