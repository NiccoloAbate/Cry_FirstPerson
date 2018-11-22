#pragma once

#define _JOINSTR(x, y) #x y
#define _JOIN(x, y) x##y
#define MAKEXMLEVENT(strName) _JOINSTR(XMLEVENT_, strName)
#define ADDXMLEVENT(eventName, funcName) ADDCONSOLECOMMAND_HIDDEN(MAKEXMLEVENT(eventName), funcName)

class CCharacterComponent;

class CDialogue
{
	friend class CDialogueManager;
public:

	bool Load(string FileName);
	bool Start(string StartName = "Start_1");

	void NextNode(int ResponseId = -1);

	CCharacterComponent *m_pCharacter = nullptr;

private:
	XmlNodeRef m_Dialogue = NULL;
	XmlNodeRef m_CurrentNode = NULL;

	void RunCurrentNode();
	void DisplayText(string Text);
	void SetResponseNum(int NumResponses);
	void DisplayResponse(int Index, string Text);
};

class CDialogueManager
{
public:

	CDialogue *m_pCurrentDialogue = nullptr;

	void Initialize();
	bool LoadDialogue(string FilePath); // FilePath is from Assets folder; Don't use file extension

	bool IsInDialogue() { return m_pCurrentDialogue != nullptr; }

	static void CallXmlEvent(string EventName, string Params = "");

};
