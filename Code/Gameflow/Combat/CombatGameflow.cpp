#include "StdAfx.h"
#include "CombatGameflow.h"

#include "ActionPhase.h"



void CCombatGameflow::Initialize()
{
	GAMEFLOW_STANDARD_INIT();

	CCombatGameflow_ActionPhase *pActionPhase = new CCombatGameflow_ActionPhase;
	pActionPhase->Initialize();
	AddPhase(pActionPhase);

	m_ActivePhase = 0;
}

void CCombatGameflow::Update(float fDeltaTime)
{


	UpdatePhases(fDeltaTime);
}
