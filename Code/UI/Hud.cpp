#include "StdAfx.h"
#include "Hud.h"

#include "Dialogue\DialogueManager.h"
#include "GamePlugin.h"

#include "Components\Player\Player.h"
#include "Components\Player\PlayerExtension.h"
#include "Components\Game\Stats.h"

#include "Game\UIController.h"
#include "UI\EntityHud.h"

void CHud::Initialize()
{
	m_pElement = gEnv->pFlashUI->GetUIElement("Hud");
	auto pDisplay = gEnv->pFlashUI->GetUIAction("Hud");
	gEnv->pFlashUI->GetUIActionManager()->StartAction(pDisplay, SUIArguments());

	m_pEventListener = new CHud::EventListener;
	m_pElement->AddEventListener(m_pEventListener, "HudEventListener");
}

void CHud::OnLevelLoaded()
{
}

void CHud::OnGameplayStart()
{
	InitializeStatBars();
}

void CHud::UpdateStaminaBar()
{
	CStatsComponent *pStatsComponent = GetStatsComponent();
	MinMaxVar<float> &Stamina = pStatsComponent->GetStamina();
	m_pStaminaBar->InitializeStat("Stamina", Stamina);
}

void CHud::InitializeStatBars()
{
	CStatsComponent *pStatsComponent = GetStatsComponent();
	MinMaxVar<float> &Stamina = pStatsComponent->GetStamina();
	m_pStaminaBar = GetEntityHudManager()->NewStatBar("Hud_StaminaBar", "Stamina", (int)((1920) / 2), 980, "Stamina", Stamina);
	m_pStaminaBar->m_bUpdatePos = false;
	//m_pStaminaBar->SetScale(1.5);
}

void CHud::UpdateStatBars()
{
	UpdateStaminaBar();
}

void CHud::ClearResponses()
{
	m_pElement->CallFunction("ClearResponses", SUIArguments());
}

void CHud::SetResponseNum(int Num)
{
	SUIArguments SetResponseNumArgs;
	SetResponseNumArgs.AddArgument<int>(Num);
	m_pElement->CallFunction("SetResponseNum", SetResponseNumArgs);
}

void CHud::SetResponse(int Index, string Text)
{
	SUIArguments SetResponseArgs;
	SetResponseArgs.AddArgument<int>(Index);
	SetResponseArgs.AddArgument<string>(Text);
	m_pElement->CallFunction("SetResponse", SetResponseArgs);
}

CEntityHudManager * CHud::GetEntityHudManager()
{
	return CGamePlugin::gGamePlugin->m_pUIController->m_pEntityHudManager;
}

CPlayerComponent * CHud::GetPlayerComponent()
{
	return CGamePlugin::gGamePlugin->GetPlayerComponent();
}

CStatsComponent * CHud::GetStatsComponent()
{
	return CGamePlugin::gGamePlugin->GetPlayerComponent()->GetStatsComponent();
}

void CHud::EventListener::OnUIEvent(IUIElement * pSender, const SUIEventDesc & event, const SUIArguments & args)
{
	if (string(event.sDisplayName) == "Dialogue_Response")
	{
		int Index;
		if (args.GetArg(0, Index))
		{
			if (CDialogue *pDialogue = CGamePlugin::gGamePlugin->m_pDialogueManager->m_pCurrentDialogue)
			{
				pDialogue->NextNode(Index + 1);
			}
		}
	}
}
