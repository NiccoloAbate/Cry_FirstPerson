#include "StdAfx.h"
#include "Player.h"

#include "GamePlugin.h"
#include "Game\CameraController.h"

void CPlayerComponent::Initialize()
{
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	m_pCamera = CGamePlugin::gGamePlugin->m_pCameraController->m_pCamera;
}

void CPlayerComponent::ProcessEvent(SEntityEvent & event)
{
}
