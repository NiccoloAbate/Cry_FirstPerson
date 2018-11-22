#include "StdAfx.h"
#include "ActionPhase.h"

#include "CombatGameflow.h"

#include <IActionMapManager.h>
#include <CryGame\IGameFramework.h>


void CCombatGameflow_ActionPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();
}

void CCombatGameflow_ActionPhase::InitializeKeyBinds(Cry::DefaultComponents::CInputComponent * pInputComponent)
{
	if (!pInputComponent)
		pInputComponent = GetPlayerInputComponent();

	pInputComponent->RegisterAction(PHASENAME, "Test", [this](int activationMode, float value) { GAMEFLOWPHASE_ACTION_STARTCHECK(); Test(); });
	pInputComponent->BindAction(PHASENAME, "Test", eAID_KeyboardMouse, EKeyId::eKI_I, false, true, false);

	gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(PHASENAME, true);
	gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(PHASENAME, false);
	gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(PHASENAME, true);
}

void CCombatGameflow_ActionPhase::Update(float fDeltaTime)
{

}

void CCombatGameflow_ActionPhase::Test()
{
	bool worked = true;
}
