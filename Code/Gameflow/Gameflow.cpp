#include "StdAfx.h"
#include "Gameflow.h"

#include "GameflowPhase.h"

#include "GamePlugin.h"
#include "GameflowManager.h"


CGameflowPhase * CGameflow::GetPhase(int Id)
{
	for (int x = 0; x < m_Phases.size(); x++)
	{
		if (m_Phases[x]->GetPhaseId() == Id)
			return m_Phases[x];
	}
	return nullptr;
}

void CGameflow::StartNextPhase(int PhaseId)
{
	m_Phases[m_ActivePhase]->OnPhaseEnd();
	SetActivePhase(PhaseId); // Sets active phase
	m_Phases[m_ActivePhase]->OnPhaseStart();
}

void CGameflow::UpdatePhases(float fDeltaTime)
{

	for (int x = 0; x < m_Phases.size(); x++)
	{
		if (x == m_ActivePhase)
			m_Phases[x]->Update(fDeltaTime);
		else
		{
			if (m_Phases[x]->GetFlags().HasFlag(CGameflow::EGameflowFlags::UPDATE_IN_BACKGROUND))
				m_Phases[x]->Update(fDeltaTime);
		}
	}

}

int CGameflow::AddPhase(CGameflowPhase * pPhase)
{
	m_Phases.push_back(pPhase);
	return m_Phases.size() - 1;
}

void CGameflow::SetActivePhaseIndex(int Index)
{
	m_Phases[m_ActivePhase]->SetActivePhase(false); // Sets old phase to inactive
	m_ActivePhase = Index;
	m_Phases[Index]->SetActivePhase(true); // Sets new phase to active
	SetActivePhase_Debug(m_Phases[Index]);
}

void CGameflow::SetActivePhase(int PhaseId)
{
	for (int i = 0; i < m_Phases.size(); ++i)
	{
		if (m_Phases[i]->GetPhaseId() == PhaseId)
			SetActivePhaseIndex(i);
	}
}

void CGameflow::SetActivePhase(CGameflowPhase * pPhase)
{
	for (int i = 0; i < m_Phases.size(); ++i)
	{
		if (m_Phases[i] == pPhase)
			SetActivePhaseIndex(i);
	}
}

//DEBUG//
void CGameflow::SetActivePhase_Debug(CGameflowPhase * pPhase)
{
	const string Message = "Set to GameflowPhase: " + m_GameflowName + ": " + pPhase->GetPhaseName();
	if (GetGameflowManager()->GetDebugMode() >= EDebugMode::CONSOLE)
	{
		gEnv->pLog->Log(Message);
	}
	if (GetGameflowManager()->GetDebugMode() >= EDebugMode::PERSISTANT)
	{
		gEnv->pGameFramework->GetIPersistantDebug()->Add2DText(Message, DEBUGTEXTSIZE, DEBUGTEXTCOLOR, DEBUGTEXTDURATION);
	}
}

CGameflowManager * CGameflow::GetGameflowManager()
{
	return CGamePlugin::gGamePlugin->m_pGameflowManager;
}

void CGameflow::SetActiveGameflow(bool bActive)
{
	if (bActive)
	{
		gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(m_GameflowName, true);
	}
	else
	{
		if (!GetFlags().HasFlag(EGameflowFlags::UPDATE_IN_BACKGROUND))
		{
			gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(m_GameflowName, false);
		}
	}
}

Cry::DefaultComponents::CInputComponent * CGameflow::GetInputComponent()
{
	return GetGameflowManager()->GetInputComponent();
}
