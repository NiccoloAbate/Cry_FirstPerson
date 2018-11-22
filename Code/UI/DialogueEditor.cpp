#include "StdAfx.h"
#include "DialogueEditor.h"

#include "GamePlugin.h"
#include "Game\UIController.h"
#include "Utils\ConsoleCommands.h"

#include "Utils\Files\FileManager.h"


void CDialogueEditor::Initialize()
{
	m_pElement = gEnv->pFlashUI->GetUIElement("DialogueEditor");
	auto pDisplay = gEnv->pFlashUI->GetUIAction("DialogueEditor");
	gEnv->pFlashUI->GetUIActionManager()->StartAction(pDisplay, SUIArguments());

	std::vector<string> DialogueFileNames = CFileManager::GetFilesInDir("Assets" + string(SLASH_SYS) + "Characters" + string(SLASH_SYS) + "Dialogues");
	for (int i = 0; i < DialogueFileNames.size(); i++)
	{
		SUIArguments AddDialogueFileNameArgs;
		AddDialogueFileNameArgs.AddArgument<string>(DialogueFileNames[i]);
		m_pElement->CallFunction("AddDialogueFileName", AddDialogueFileNameArgs);
	}

	m_pEventListener = new CDialogueEditor::EventListener;
	m_pEventListener->m_pDialogueEditor = this;
	m_pElement->AddEventListener(m_pEventListener, "DialogueEditorEventListener");

	m_pMouseHandler = new MouseHandler;
	m_pMouseHandler->m_pDialogueEditor = this;
	gEnv->pHardwareMouse->AddListener(m_pMouseHandler);

	SetActive(false);
}

void CDialogueEditor::SetActive(bool Active)
{
	if (Active)
		gEnv->pConsole->ExecuteString("t_scale 0");
	else
		gEnv->pConsole->ExecuteString("t_scale 1");

	SetVisible(Active);
}

void CDialogueEditor::SetVisible(bool Visible)
{
	SUIArguments SetVisibleArgs;
	SetVisibleArgs.AddArgument<bool>(Visible);
	m_pElement->CallFunction("SetVisible", SetVisibleArgs);
}

void CDialogueEditor::Save_Start(string DialogueName)
{
	m_Dialogue = gEnv->pSystem->CreateXmlNode(DialogueName);
	m_CurrentNode = m_Dialogue;
}

void CDialogueEditor::Save_StartNode(string Name, string Next)
{
}


void CDialogueEditor::EventListener::OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args)
{
	if (string(event.sDisplayName) == "Save_Start")
	{
		string DialogueName;
		if (args.GetArg(0, DialogueName))
		{
			m_pDialogueEditor->Save_Start(DialogueName);
		}
	}
	if (string(event.sDisplayName) == "Save_Node")
	{
		int NodeType;
		string Name;
		if (args.GetArg(0, NodeType))
		{
			args.GetArg(1, Name);
			switch (NodeType)
			{
			default:
				break;
			case NODETYPE::NODETYPE_START:
			{
				string Next;
				args.GetArg(2, Next);

				m_pDialogueEditor->Save_StartNode(Name, Next);
			}
			break;
			}
		}
	}
}

void CDialogueEditor::MouseHandler::OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT eHardwareMouseEvent, int wheelDelta)
{
	switch (eHardwareMouseEvent)
	{
	case EHARDWAREMOUSEEVENT::HARDWAREMOUSEEVENT_RBUTTONDOWN:
	{
		m_pDialogueEditor->m_pElement->CallFunction("OnRightClick", SUIArguments());
	}
	break;
	default:
		break;
	}
}



void CC_DialogueEditor_SetVisible(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1)
	CC_GETARG(bool, Visible, 1)
	CDialogueEditor *pDialogueEditor = CGamePlugin::gGamePlugin->m_pUIController->m_pDialogueEditor;
	pDialogueEditor->SetVisible(Visible);
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("DialogueEditor_SetVisible", 1), CC_DialogueEditor_SetVisible)

void CC_DialogueEditor_SetActive(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1)
	CC_GETARG(bool, Active, 1)
	CDialogueEditor *pDialogueEditor = CGamePlugin::gGamePlugin->m_pUIController->m_pDialogueEditor;
	pDialogueEditor->SetActive(Active);
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("DialogueEditor_SetActive", 1), CC_DialogueEditor_SetActive)

#if MODE_DIALOGUEEDITOR
void CC_QUICK(CC_Args pArgs)
{
	static bool Active = false;
	Active = !Active;
	CDialogueEditor *pDialogueEditor = CGamePlugin::gGamePlugin->m_pUIController->m_pDialogueEditor;
	pDialogueEditor->SetActive(Active);
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("QUICK", 0), CC_QUICK)
#endif