#include "StdAfx.h"
#include "SpeechBubbles.h"

#include "Utils\ConsoleCommands.h"

void CSpeechBubbleManager::Initialize()
{

	m_pElement = gEnv->pFlashUI->GetUIElement("SpeechBubbles");
	auto pDisplay = gEnv->pFlashUI->GetUIAction("SpeechBubbles");
	gEnv->pFlashUI->GetUIActionManager()->StartAction(pDisplay, SUIArguments());

	SUIArguments DeleteBubbleArgs;
	DeleteBubbleArgs.AddArgument<int>(0);
	m_pElement->CallFunction("DeleteBubble", DeleteBubbleArgs);

}

void CSpeechBubbleManager::Update(float fFrameTime)
{
	for (int i = 0; i < m_pSpeechBubbles.size(); i++)
	{
		if(m_pSpeechBubbles[i]->m_bUpdatePos || m_pSpeechBubbles[i]->m_Life != -1)
			m_pSpeechBubbles[i]->Update(fFrameTime);
	}
}

CSpeechBubble* CSpeechBubbleManager::NewBubble(string Text, float Life)
{
	CSpeechBubble *pSpeechBubble = new CSpeechBubble;
	pSpeechBubble->m_pManager = this;
	pSpeechBubble->m_Life = Life;
	m_pSpeechBubbles.push_back(pSpeechBubble);

	SUIArguments NewBubbleArgs;
	NewBubbleArgs.AddArgument<int>(0);
	NewBubbleArgs.AddArgument<int>(0);
	NewBubbleArgs.AddArgument<float>(1.0f);
	TUIData NewBubbleReturn;
	m_pElement->CallFunction("NewBubble", NewBubbleArgs, &NewBubbleReturn);
	int Index;
	NewBubbleReturn.GetValueWithConversion(Index);
	pSpeechBubble->m_Index = Index;

	SUIArguments SetBubbleTextArgs;
	SetBubbleTextArgs.AddArgument<int>(Index);
	SetBubbleTextArgs.AddArgument<string>(Text);
	m_pElement->CallFunction("SetBubbleText", SetBubbleTextArgs);

	pSpeechBubble->Update(0);
	return pSpeechBubble;
}

void CSpeechBubble::Update(float fFrameTime)
{
	if (m_bUpdatePos)
	{
		Vec3 TargetPos;
		if (m_bPosFunc)
			TargetPos = m_PosFunc();
		else
			TargetPos = m_Pos;
		Vec3 ScreenPos;
		float Size = 1;

		bool bOnScreen = gEnv->pRenderer->GetCamera().Project(TargetPos, ScreenPos);
		if (!bOnScreen)
		{
			if (ScreenPos.x < 0)
				ScreenPos.x = 0;
			else
				ScreenPos.x = 1920;
			if (ScreenPos.y < 0)
				ScreenPos.y = 0;
			else
				ScreenPos.y = 1080;
		}

		SUIArguments UpdateBubbleArgs;
		UpdateBubbleArgs.AddArgument<int>(m_Index);
		UpdateBubbleArgs.AddArgument<int>((int)ScreenPos.x);
		UpdateBubbleArgs.AddArgument<int>((int)ScreenPos.y);
		UpdateBubbleArgs.AddArgument<float>(1.0f); //ScreenPos.z
		m_pManager->m_pElement->CallFunction("UpdateBubble", UpdateBubbleArgs);
	}

	if (m_Life == -1)
		return;
	m_Life -= fFrameTime;
	if (m_Life <= 0)
		Destroy();
}

void CSpeechBubble::Destroy()
{
	SUIArguments DeleteBubbleArgs;
	DeleteBubbleArgs.AddArgument<int>(m_Index);
	m_pManager->m_pElement->CallFunction("DeleteBubble", DeleteBubbleArgs);
	std::vector<CSpeechBubble*> & SpeechBubbles = m_pManager->m_pSpeechBubbles;
	auto ThisIndex = std::find(SpeechBubbles.begin(), SpeechBubbles.end(), this);
	SpeechBubbles.erase(ThisIndex);
	delete this;
}

void CSpeechBubble::SetText(string Text)
{
	SUIArguments SetBubbleTextArgs;
	SetBubbleTextArgs.AddArgument<int>(m_Index);
	SetBubbleTextArgs.AddArgument<string>(Text);
	m_pManager->m_pElement->CallFunction("SetBubbleText", SetBubbleTextArgs);
}

#include "GamePlugin.h"
#include "Game\UIController.h"
#include "Components\Hive\Hive.h"
void CC_TestBubble_1(CC_Args pArgs)
{
	auto pBubble = CGamePlugin::gGamePlugin->m_pUIController->m_pSpeechBubbleManager->NewBubble("WORK YOU FAT CUCK", 2);
	pBubble->SetPos(CGamePlugin::gGamePlugin->m_pHive->GetEntity()->GetWorldPos());
}
ADDCONSOLECOMMAND("TestBubble_1", CC_TestBubble_1)
void CC_TestBubble_2(CC_Args pArgs)
{
	auto pBubble = CGamePlugin::gGamePlugin->m_pUIController->m_pSpeechBubbleManager->NewBubble("WORK YOU FAT CUCK");
	pBubble->SetPosFunc([]() {return CGamePlugin::gGamePlugin->m_pHive->GetEntity()->GetWorldPos(); });
}
ADDCONSOLECOMMAND("TestBubble_2", CC_TestBubble_2)
