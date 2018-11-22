#include "StdAfx.h"
#include "PlayerPhase.h"

#include "CombatGameflow.h"

#include <IActionMapManager.h>
#include <CryGame\IGameFramework.h>


void CCombatGameflow_PlayerPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();
}

void CCombatGameflow_PlayerPhase::InitializeKeyBinds()
{
	Cry::DefaultComponents::CInputComponent	*pInputComponent = GetPlayerInputComponent();

	pInputComponent->RegisterAction(PHASENAME, "Test", [this](int activationMode, float value) { GAMEFLOWPHASE_ACTION_STARTCHECK(); Test(); });
	pInputComponent->BindAction(PHASENAME, "Test", eAID_KeyboardMouse, EKeyId::eKI_I, false, true, false);

	gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(PHASENAME, true);
	gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(PHASENAME, false);
	gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(PHASENAME, true);
}

void CCombatGameflow_PlayerPhase::Update(float fDeltaTime)
{

}

void CCombatGameflow_PlayerPhase::Test()
{
	bool worked = true;
}
