#include "StdAfx.h"
#include "GameflowManager.h"

#include "Gameflow.h"
#include "Combat\CombatGameflow.h"

void CGameflowManager::Initialize()
{
	//Initialize Input Component
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();

	if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
		m_pInputComponent = pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	// Gameflows

	AddNewGameflow<CCombatGameflow>();

	SetActiveGameflowIndex(0);
}

void CGameflowManager::Update(float fDeltaTime)
{

	for (int x = 0; x < m_Gameflows.size(); x++)
	{
		if (x == m_ActiveGameflow)
			m_Gameflows[x]->Update(fDeltaTime);
		else
		{
			if (m_Gameflows[x]->GetFlags().HasFlag(CGameflow::EGameflowFlags::UPDATE_IN_BACKGROUND))
				m_Gameflows[x]->Update(fDeltaTime);
		}
	}

}

CGameflow * CGameflowManager::GetGameflowFromId(int Id)
{
	for (int i = 0; i < m_Gameflows.size(); i++)
	{
		if (m_Gameflows[i]->GetGameflowId() == Id)
			return m_Gameflows[i];
	}
	return nullptr;
}

int CGameflowManager::AddGameflow(CGameflow * pGameflow)
{
	m_Gameflows.push_back(pGameflow);
	return m_Gameflows.size() - 1;
}

void CGameflowManager::SetActiveGameflowIndex(int Index)
{
	m_ActiveGameflow = Index;
	SetActiveGameflow_Debug(m_Gameflows[Index]);
}

void CGameflowManager::SetActiveGameflow(int Id)
{
	for (int i = 0; i < m_Gameflows.size(); i++)
	{
		if (m_Gameflows[i]->GetGameflowId() == Id)
		{
			m_ActiveGameflow = i;
			SetActiveGameflow(m_Gameflows[i]);
			return;
		}
	}
}

void CGameflowManager::SetActiveGameflow(CGameflow * pGameflow)
{
	for (int i = 0; i < m_Gameflows.size(); i++)
	{
		if (m_Gameflows[i] == pGameflow)
		{
			m_ActiveGameflow = i;
			SetActiveGameflow(pGameflow);
			return;
		}
	}
}

void CGameflowManager::SetActiveGameflow_Debug(CGameflow * pGameflow)
{
	if (GetDebugMode() >= EDebugMode::CONSOLE)
	{
		gEnv->pLog->Log("Set to Gameflow: " + pGameflow->GetGameflowName());
	}
}

REGISTERDEBUGEXTENSION(CGameflowManager, "Gameflow", EDebugMode::CONSOLE);
