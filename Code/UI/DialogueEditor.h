#pragma once

#include "FlashUI\FlashUI.h"
#include "FlashUI\FlashUIAction.h"
#include "FlashUI\FlashUIElement.h"

#include "CryInput\IHardwareMouse.h"

#define MODE_DIALOGUEEDITOR 1

class CDialogueEditor
{
public:

	IUIElement *m_pElement;
	void Initialize();

	void SetActive(bool Active);
	void SetVisible(bool Visible);

private:

	XmlNodeRef m_Dialogue;
	XmlNodeRef m_CurrentNode;

	void Save_Start(string DialogueName);
	void Save_StartNode(string Name, string Next);

	enum NODETYPE
	{
		NODETYPE_START = 0,
		NODETYPE_BRANCH,
		NODETYPE_END,
	};

	class EventListener : public IUIElementEventListener
	{
	public:
		CDialogueEditor *m_pDialogueEditor;
		virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args) override;
	};
	EventListener *m_pEventListener;

	class MouseHandler : public IHardwareMouseEventListener
	{
	public:
		CDialogueEditor *m_pDialogueEditor;
		virtual void OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT eHardwareMouseEvent, int wheelDelta = 0) override;
	};
	MouseHandler *m_pMouseHandler;
};
