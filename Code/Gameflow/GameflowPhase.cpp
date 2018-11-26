#include "StdAfx.h"
#include "GameflowPhase.h"

#include "GamePlugin.h"
#include "GameflowManager.h"
#include "Gameflow.h"

#include "CryGame\IGameFramework.h"
#include "IActionMapManager.h"

CGameflowManager * CGameflowPhase::GetGameflowManager()
{
	return CGamePlugin::gGamePlugin->m_pGameflowManager;
}

bool CGameflowPhase::IsActivePhase()
{
	return (GetGameflowManager()->GetActiveGameflow()->GetActivePhase() == this);
}

Cry::DefaultComponents::CInputComponent * CGameflowPhase::GetInputComponent()
{
	return GetGameflowManager()->GetInputComponent();
}

void CGameflowPhase::SetActivePhase(bool bActive)
{
	if(bActive)
	{
		gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(m_PhaseName, true);
	}
	else
	{
		if (!GetFlags().HasFlag(EGameflowPhaseFlags::UPDATE_IN_BACKGROUND))
		{
			gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(m_PhaseName, false);
		}
	}
}
