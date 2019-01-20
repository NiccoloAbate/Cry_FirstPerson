#include "StdAfx.h"
#include "PlayerPhase.h"

#include "CombatGameflow.h"
#include "CombatGameflowPhases.h"

#include "Components\Characters\Character.h"
#include "Components\Game\Stats.h"


void CCombatGameflow_PlayerPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();
}

void CCombatGameflow_PlayerPhase::OnPhaseStart()
{
	CCharacterComponent *pCharacter = ((CCombatGameflow*)GetParentGameflow())->GetCharacter();
	pCharacter->GetStatsComponent()->GetStamina().ResetToMax();
}

void CCombatGameflow_PlayerPhase::InitializeKeyBinds()
{
	Cry::DefaultComponents::CInputComponent	*pInputComponent = GetInputComponent();

	pInputComponent->RegisterAction(PHASENAME, "Test", [this](int activationMode, float value) { Test(); });
	pInputComponent->BindAction(PHASENAME, "Test", eAID_KeyboardMouse, EKeyId::eKI_I, false, true, false);

	pInputComponent->RegisterAction(PHASENAME, "EndTurn", [this](int activationMode, float value) { if(((CCombatGameflow*)GetParentGameflow())->IsPlayerOutOfMoves()) EndPhase(); }); // Ends turn if player is out of moves
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
