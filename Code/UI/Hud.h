#pragma once

#include "FlashUI\FlashUI.h"
#include "FlashUI\FlashUIAction.h"
#include "FlashUI\FlashUIElement.h"

#include "Types\MinMaxVar.h"


class CEntityHudElement_StatBar;
class CEntityHudManager;
class CPlayerComponent;
class CStatsComponent_Deprecated;

class CHud
{
public:

	IUIElement *m_pElement;
	void Initialize();
	void OnLevelLoaded();
	void OnGameplayStart();

	// Stats
	CEntityHudElement_StatBar *m_pStaminaBar = nullptr;
	void UpdateStaminaBar();
	void InitializeStatBars(); // in OnGameplayStart()
	void UpdateStatBars();
	//~Stats

	// Responses
	void ClearResponses();
	void SetResponseNum(int Num);
	void SetResponse(int Index, string Text);
	//~Responses
private:
	class EventListener : public IUIElementEventListener
	{
	public:
		virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;
	};
	EventListener *m_pEventListener;

	CEntityHudManager* GetEntityHudManager();
	CPlayerComponent* GetPlayerComponent();
	CStatsComponent_Deprecated* GetStatsComponent();
};