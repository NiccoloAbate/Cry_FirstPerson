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
	m_ActivePhase = Index;
	SetActivePhase_Debug(m_Phases[Index]);
}

void CGameflow::SetActivePhase(int Id)
{
	for (int i = 0; i < m_Phases.size(); ++i)
	{
		if (m_Phases[i]->GetPhaseId() == Id)
		{
			m_ActivePhase = i;
			SetActivePhase_Debug(m_Phases[i]);
			return;
		}
	}
}

void CGameflow::SetActivePhase(CGameflowPhase * pPhase)
{
	for (int i = 0; i < m_Phases.size(); ++i)
	{
		if (m_Phases[i] == pPhase)
		{
			m_ActivePhase = i;
			SetActivePhase_Debug(m_Phases[i]);
			return;
		}
	}
}

void CGameflow::SetActivePhase_Debug(CGameflowPhase * pPhase)
{
	if (GetGameflowManager()->GetDebugMode() >= EDebugMode::CONSOLE)
	{
		gEnv->pLog->Log("Set to GameflowPhase: " + m_GameflowName + ": " + pPhase->GetPhaseName());
	}
}

CGameflowManager * CGameflow::GetGameflowManager()
{
	return CGamePlugin::gGamePlugin->m_pGameflowManager;
}
