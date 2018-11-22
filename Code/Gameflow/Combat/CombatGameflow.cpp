#include "StdAfx.h"
#include "CombatGameflow.h"

#include "Gameflow\GameflowPhase.h"

#include "PlayerPhase.h"
#include "PlayerEndPhase.h"
#include "EnemyPhase.h"
#include "EnemyEndPhase.h"



void CCombatGameflow::Initialize()
{
	GAMEFLOW_STANDARD_INIT();

	AddNewPhase<CCombatGameflow_PlayerPhase>();
	AddNewPhase<CCombatGameflow_PlayerEndPhase>();
	AddNewPhase<CCombatGameflow_EnemyPhase>();
	AddNewPhase<CCombatGameflow_EnemyEndPhase>();

	SetActivePhaseIndex(0);
}

void CCombatGameflow::Update(float fDeltaTime)
{


	UpdatePhases(fDeltaTime);
}
