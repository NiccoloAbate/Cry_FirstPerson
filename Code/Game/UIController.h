#pragma once

#include "FlashUI\FlashUI.h"
#include "FlashUI\FlashUIAction.h"
#include "FlashUI\FlashUIElement.h"

class CSpeechBubbleManager;
class CEntityHudManager;
class CHud;
class CDialogueEditor;

class CUIController
{
public:

	CSpeechBubbleManager *m_pSpeechBubbleManager;
	CEntityHudManager *m_pEntityHudManager;
	CHud *m_pHud;
	CDialogueEditor *m_pDialogueEditor;

	void Initialize();
	void OnLevelLoaded();
	void OnGameplayStart();
	void Update(float fFrameTime);

};