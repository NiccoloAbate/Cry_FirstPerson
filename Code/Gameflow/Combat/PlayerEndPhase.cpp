#include "StdAfx.h"
#include "PlayerEndPhase.h"

#include "CombatGameflow.h"
#include "EnemyPhase.h"

void CCombatGameflow_PlayerEndPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();
}

void CCombatGameflow_PlayerEndPhase::OnPhaseStart()
{
	GetParentGameflow()->StartNextPhase(Gameflow::CombatGameflow::Phases::ENEMY);
}

void CCombatGameflow_PlayerEndPhase::InitializeKeyBinds()
{
}

void CCombatGameflow_PlayerEndPhase::Update(float fDeltaTime)
{
}
