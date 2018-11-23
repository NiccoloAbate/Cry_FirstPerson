#include "StdAfx.h"
#include "PlayerPhase.h"

#include "CombatGameflow.h"
#include "CombatGameflowPhases.h"


void CCombatGameflow_PlayerPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();
}

void CCombatGameflow_PlayerPhase::InitializeKeyBinds()
{
	Cry::DefaultComponents::CInputComponent	*pInputComponent = GetPlayerInputComponent();

	pInputComponent->RegisterAction(PHASENAME, "Test", [this](int activationMode, float value) { Test(); });
	pInputComponent->BindAction(PHASENAME, "Test", eAID_KeyboardMouse, EKeyId::eKI_I, false, true, false);

	pInputComponent->RegisterAction(PHASENAME, "EndTurn", [this](int activationMode, float value) { EndPhase(); });
	pInputComponent->BindAction(PHASENAME, "EndTurn", eAID_KeyboardMouse, EKeyId::eKI_Enter, false, true, false);
}

void CCombatGameflow_PlayerPhase::Update(float fDeltaTime)
{

}

void CCombatGameflow_PlayerPhase::EndPhase()
{
	GetParentGameflow()->StartNextPhase(Gameflow::CombatGameflow::Phases::PLAYER_END);
}

void CCombatGameflow_PlayerPhase::Test()
{
	bool worked = true;
}
