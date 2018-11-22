#include "StdAfx.h"
#include "DialogueManager.h"

#include "Utils\ConsoleCommands.h"
#include "Build.h"

#include "Components\Characters\Character.h"

#include "GamePlugin.h"
#include "Game\UIController.h"
#include "UI\Hud.h"

bool CDialogue::Load(string FileName)
{
	m_Dialogue = gEnv->pSystem->LoadXmlFromFile(FileName);
	if (!m_Dialogue)
		return false;
	return true;
}

bool CDialogue::Start(string StartName)
{
	if (!m_Dialogue)
		return false;
	m_CurrentNode = m_Dialogue->findChild(StartName);
	if(!m_CurrentNode)
		return false;
	RunCurrentNode();
	return true;
}

void CDialogue::RunCurrentNode()
{
	if (!m_CurrentNode)
		return;

	XmlNodeRef EventNode = m_CurrentNode->findChild("Event");
	if (EventNode)
	{
		if (string Event = EventNode->getAttr("Event"))
		{
			CDialogueManager::CallXmlEvent(Event);
		}
	}

	XmlNodeRef TextNode = m_CurrentNode->findChild("Text");
	if (TextNode)
	{
		if (string Text = TextNode->getAttr("Text"))
		{
			DisplayText(Text);
		}
	}

	if (!m_CurrentNode->findChild("Response_1"))
	{
		if(!TextNode)
			NextNode();
		return;
	}
	
	int NumResponses = 0;
	while (m_CurrentNode->findChild("Response_" + ToString(NumResponses + 1)))
	{
		NumResponses++;
	}
	SetResponseNum(NumResponses);
	for (int i = 1; i <= NumResponses; i++)
	{

		XmlNodeRef ResponseNode = m_CurrentNode->findChild("Response_" + ToString(i));
		if (XmlNodeRef TextNode = ResponseNode->findChild("Text"))
		{
			if (string Text = TextNode->getAttr("Text"))
			{
				DisplayResponse(i - 1, Text);
			}
		}

	}
}

void CDialogue::DisplayText(string Text)
{
	gEnv->pLog->Log(Text);
	if (m_pCharacter)
		m_pCharacter->Speak(Text);
}

void CDialogue::SetResponseNum(int NumResponses)
{
	CGamePlugin::gGamePlugin->m_pUIController->m_pHud->SetResponseNum(NumResponses);
}

void CDialogue::DisplayResponse(int Index, string Text)
{
	CGamePlugin::gGamePlugin->m_pUIController->m_pHud->SetResponse(Index, Text);
}

void CDialogue::NextNode(int ResponseId)
{
	if (!m_CurrentNode)
		return;
	XmlNodeRef InfoNode;
	if (ResponseId == -1)
		InfoNode = m_CurrentNode->findChild("Info");
	else
	{
		XmlNodeRef ResponseNode = m_CurrentNode->findChild("Response_" + ToString(ResponseId));
		if(ResponseNode)
			InfoNode = ResponseNode->findChild("Info");
	}
	
	if (InfoNode)
	{
		if (string Next = InfoNode->getAttr("Next"))
		{
			XmlNodeRef NextNode = m_Dialogue->findChild(Next);
			if (NextNode)
			{
				m_CurrentNode = NextNode;
				RunCurrentNode();
			}
		}
	}
}

#include "Components\Game\GameplayEntity.h"
#include "Utils\DebugUtils.h"
#include "Utils\Files\FileManager.h"
void CDialogueManager::Initialize()
{
	std::vector<string> DialogueFileNames = CFileManager::GetFilesInDir("Assets" + string(SLASH_SYS) + "Characters" + string(SLASH_SYS) + "Dialogues");
	auto DebugInfo = GETDEBUGINFO(DialogueFileNames);
	bool worked = true;
}

bool CDialogueManager::LoadDialogue(string FilePath)
{
	DEBUG_CODE(FilePath += ".xml";)
	m_pCurrentDialogue = new CDialogue;
	return m_pCurrentDialogue->Load(FilePath);
}

void CDialogueManager::CallXmlEvent(string EventName, string Params)
{
	const string EventCall = "XMLEVENT_" + EventName + " " + Params;
	gEnv->pConsole->ExecuteString(EventCall);
}

void CC_NextNode(CC_Args pArgs)
{
	if(pArgs->GetArgCount() == 1)
		CGamePlugin::gGamePlugin->m_pDialogueManager->m_pCurrentDialogue->NextNode();
	else
	{
		CC_GETARG(int, ResponseId, 1);
		CGamePlugin::gGamePlugin->m_pDialogueManager->m_pCurrentDialogue->NextNode(ResponseId);
	}
}
ADDCONSOLECOMMAND("NextNode", CC_NextNode)

void XMLEVENT_Null(CC_Args pArgs)
{
	CGamePlugin::gGamePlugin->m_pUIController->m_pHud->ClearResponses();
}
ADDXMLEVENT("null", XMLEVENT_Null)
