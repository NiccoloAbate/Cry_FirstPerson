#include "StdAfx.h"
#include "EnemyPhase.h"

#include "CombatGameflow.h"
#include "EnemyEndPhase.h"

void CCombatGameflow_EnemyPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();
}

void CCombatGameflow_EnemyPhase::OnPhaseStart()
{
	GetParentGameflow()->StartNextPhase(Gameflow::CombatGameflow::Phases::ENEMY_END);
}

void CCombatGameflow_EnemyPhase::InitializeKeyBinds()
{
}

void CCombatGameflow_EnemyPhase::Update(float fDeltaTime)
{
}
