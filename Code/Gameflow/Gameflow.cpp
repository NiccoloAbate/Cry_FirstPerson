#include "StdAfx.h"
#include "Gameflow.h"

#include "GameflowPhase.h"


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
