#include "StdAfx.h"
#include "EnemyEndPhase.h"

#include "CombatGameflow.h"
#include "PlayerPhase.h"

void CCombatGameflow_EnemyEndPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();
}

void CCombatGameflow_EnemyEndPhase::OnPhaseStart()
{
	GetParentGameflow()->StartNextPhase(Gameflow::CombatGameflow::Phases::PLAYER);
}

void CCombatGameflow_EnemyEndPhase::InitializeKeyBinds()
{
}

void CCombatGameflow_EnemyEndPhase::Update(float fDeltaTime)
{
}
