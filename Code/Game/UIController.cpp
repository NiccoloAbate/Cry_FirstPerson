#include "StdAfx.h"
#include "UIController.h"

#include "UI\SpeechBubbles.h"
#include "UI\EntityHud.h"
#include "UI\Hud.h"
#include "UI\DialogueEditor.h"


void CUIController::Initialize()
{
	m_pSpeechBubbleManager = new CSpeechBubbleManager;
	m_pSpeechBubbleManager->Initialize();

	m_pEntityHudManager = new CEntityHudManager;
	m_pEntityHudManager->Initialize();

	m_pHud = new CHud;
	m_pHud->Initialize();

	m_pDialogueEditor = new CDialogueEditor();
	m_pDialogueEditor->Initialize();

	bool twerked = true;
}

void CUIController::OnLevelLoaded()
{
	m_pHud->OnLevelLoaded();
}

void CUIController::OnGameplayStart()
{
	m_pHud->OnGameplayStart();
}

void CUIController::Update(float fFrameTime)
{
	m_pSpeechBubbleManager->Update(fFrameTime);
	m_pEntityHudManager->Update(fFrameTime);
}

