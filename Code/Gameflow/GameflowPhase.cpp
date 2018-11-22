#include "StdAfx.h"
#include "GameflowPhase.h"

#include "GamePlugin.h"
#include "GameflowManager.h"
#include "Gameflow.h"


CGameflowManager * CGameflowPhase::GetGameflowManager()
{
	return CGamePlugin::gGamePlugin->m_pGameflowManager;
}

bool CGameflowPhase::IsActivePhase()
{
	return (GetGameflowManager()->GetActiveGameflow()->GetActivePhase() == this);
}

Cry::DefaultComponents::CInputComponent * CGameflowPhase::GetPlayerInputComponent()
{
	if (IEntity *pPlayer = CGamePlugin::gGamePlugin->GetActivePlayer())
	{
		return pPlayer->GetComponent<Cry::DefaultComponents::CInputComponent>();
	}
	return nullptr;
}
