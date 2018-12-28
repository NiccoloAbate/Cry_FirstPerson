#include "StdAfx.h"
#include "CombatGameflow.h"

#include "Gameflow\GameflowPhase.h"

#include "PlayerPhase.h"
#include "PlayerEndPhase.h"
#include "EnemyPhase.h"
#include "EnemyEndPhase.h"

#include "Components\SpawnPoint.h"
#include "GamePlugin.h"
#include "Components\Player\Player.h"
#include "Components\Characters\Character.h"
#include "Components\Game\Stats.h"



bool CCombatGameflow::IsPlayerOutOfMoves()
{
	bool bOutOfMoves = true;

	if (m_pCharacter->GetStatsComponent()->GetStamina() > 0)
		bOutOfMoves = false;

	return bOutOfMoves;
}

void CCombatGameflow::Initialize()
{
	GAMEFLOW_STANDARD_INIT();

	SEntitySpawnParams CharacterSpawnParams;
	CharacterSpawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	CharacterSpawnParams.sName = "PlayerCharacter";

	if (IEntity *pEntity = gEnv->pEntitySystem->SpawnEntity(CharacterSpawnParams))
	{
		if (!gEnv->IsEditor())
			CSpawnPointComponent::FindFirstSpawnPoint()->SpawnEntity(pEntity);
		if (m_pCharacter = pEntity->GetOrCreateComponent<CCharacterComponent>())
		{
			CGamePlugin::gGamePlugin->GetPlayerComponent()->ExtendTo((CPlayerExtension*)m_pCharacter->GetOrCreatePlayerExtension());
		}
	}

	SEntitySpawnParams TestSpawnParams;
	TestSpawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	TestSpawnParams.sName = "Test";
	if (IEntity *pEntity = gEnv->pEntitySystem->SpawnEntity(TestSpawnParams))
	{
		if (!gEnv->IsEditor())
			CSpawnPointComponent::FindFirstSpawnPoint()->SpawnEntity(pEntity);
		pEntity->GetOrCreateComponent<CCharacterComponent>();
	}

	AddNewPhase<CCombatGameflow_PlayerPhase>();
	AddNewPhase<CCombatGameflow_PlayerEndPhase>();
	AddNewPhase<CCombatGameflow_EnemyPhase>();
	AddNewPhase<CCombatGameflow_EnemyEndPhase>();

	SetActivePhaseIndex(0);
}

void CCombatGameflow::InitializeKeyBinds()
{
	Cry::DefaultComponents::CInputComponent *pInputComponent = GetInputComponent();

	pInputComponent->RegisterAction(GAMEFLOWNAME, "Quit", [](int activationMode, float value) { gEnv->pConsole->ExecuteString("quit"); });
	pInputComponent->BindAction(GAMEFLOWNAME, "Quit", eAID_KeyboardMouse, eKI_Escape, true, false, false);
}

void CCombatGameflow::Update(float fDeltaTime)
{
	UpdatePhases(fDeltaTime);
}
