#include "StdAfx.h"
#include "PlayerExtension.h"

#include "Player.h"

#include "GamePlugin.h"
#include "Game\UIController.h"
#include "UI\Hud.h"

#include "Utils/Utils.h"

void CPlayerExtension::Release()
{
	m_pPlayerComponent->GetCameraEntity();
	m_pPlayerComponent = nullptr;
	m_bExtended = false;
	OnReleased();
}

void CPlayerExtension::ExtendFrom(CPlayerComponent * pPlayerComponent)
{
	m_pPlayerComponent = pPlayerComponent;
	m_pPlayerComponent->GetCameraEntity()->DetachThis(); // To remove
	m_pEntity->AttachChild(m_pPlayerComponent->GetCameraEntity());
	m_bExtended = true;
	OnExtended();
}

void CPlayerExtension::FaceAt(IEntity* pEntity, float fTime)
{
	if(fTime == 0)
		Utils::EntityFaceAt(m_pEntity, pEntity);
}

void CPlayerExtension::SetViewDir(Vec3 ViewDir, float fTime)
{
	const Quat orientation = Quat::CreateRotationVDir(ViewDir);
	if(fTime == 0)
		m_pEntity->SetRotation(orientation);
}

void CPlayerExtension::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type)
{
	switch (type)
	{
	case EInputFlagType::Hold:
	{
		if (activationMode == eIS_Released)
		{
			m_InputFlags.RemoveFlags(flags);
		}
		else
		{
			m_InputFlags.AddFlags(flags);
		}
	}
	break;
	case EInputFlagType::Toggle:
	{
		if (activationMode == eIS_Released)
		{
			m_InputFlags.ToggleFlags(flags);
		}
	}
	break;
	}
}

CHud * CPlayerExtension::GetHud()
{
	return CGamePlugin::gGamePlugin->m_pUIController->m_pHud;
}
