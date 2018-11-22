#include "StdAfx.h"
#include "EntityHud.h"

void CEntityHudManager::Initialize()
{
	m_pElement = gEnv->pFlashUI->GetUIElement("Entityhud");
	auto pDisplay = gEnv->pFlashUI->GetUIAction("Entityhud");
	gEnv->pFlashUI->GetUIActionManager()->StartAction(pDisplay, SUIArguments());
}

void CEntityHudManager::Update(float fFrameTime)
{
	for (int i = 0; i < m_pHudElements.size(); i++)
	{
		if (m_pHudElements[i]->m_bUpdatePos)
			m_pHudElements[i]->Update(fFrameTime);
	}
}

CEntityHudElement_StatBar * CEntityHudManager::NewStatBar(string Name, string Type, int X, int Y, string StatName, CEntityHudElement_StatBar::Stat Stat)
{
	CEntityHudElement_StatBar *pStatBar = new CEntityHudElement_StatBar;
	pStatBar->m_pManager = this;
	pStatBar->m_StatName = StatName;
	pStatBar->m_Stat = Stat;
	m_pHudElements.push_back(pStatBar);

	SUIArguments NewStatBarArgs;
	NewStatBarArgs.AddArgument<string>(Name);
	NewStatBarArgs.AddArgument<string>(Type);
	NewStatBarArgs.AddArgument<int>(X);
	NewStatBarArgs.AddArgument<int>(Y);
	NewStatBarArgs.AddArgument<string>(StatName);
	NewStatBarArgs.AddArgument<float>(Stat.Min);
	NewStatBarArgs.AddArgument<float>(Stat.Max);
	NewStatBarArgs.AddArgument<float>(Stat.Current);
	TUIData NewStatBarReturn;
	m_pElement->CallFunction("NewStatBar", NewStatBarArgs, &NewStatBarReturn);
	int Index;
	NewStatBarReturn.GetValueWithConversion(Index);
	pStatBar->m_Index = Index;

	return pStatBar;
}

void CEntityHudElement_StatBar::SetVisible(bool Visible)
{
	SUIArguments SetStatBarVisibleArgs;
	SetStatBarVisibleArgs.AddArgument<int>(m_Index);
	SetStatBarVisibleArgs.AddArgument<bool>(Visible);
	m_pManager->m_pElement->CallFunction("SetStatBarVisible", SetStatBarVisibleArgs);
}

void CEntityHudElement_StatBar::Update(float fFrameTime)
{
	if (!m_bUpdatePos || m_bIsHidden)
		return;
	
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
		if (m_bIsOnScreen)
		{
			m_bIsOnScreen = false;
			SetVisible(false);
		}
		return;
	}
	
	if (!m_bIsOnScreen)
	{
		m_bIsOnScreen = true;
		SetVisible(true);
	}

	SUIArguments SetStatBarPosArgs;
	SetStatBarPosArgs.AddArgument<int>(m_Index);
	SetStatBarPosArgs.AddArgument<int>((int)ScreenPos.x);
	SetStatBarPosArgs.AddArgument<int>((int)ScreenPos.y);
	m_pManager->m_pElement->CallFunction("SetStatBarPos", SetStatBarPosArgs);
}

void CEntityHudElement_StatBar::SetScale(float Scale)
{
	SUIArguments SetStatBarScaleArgs;
	SetStatBarScaleArgs.AddArgument<int>(m_Index);
	SetStatBarScaleArgs.AddArgument<float>(Scale);
	m_pManager->m_pElement->CallFunction("SetStatBarScale", SetStatBarScaleArgs);
}

void CEntityHudElement_StatBar::InitializeStat(string StatName, Stat stat)
{
	m_StatName = StatName;
	m_Stat = stat;

	SUIArguments SetStatBarStatArgs;
	SetStatBarStatArgs.AddArgument<int>(m_Index);
	SetStatBarStatArgs.AddArgument<string>(StatName);
	SetStatBarStatArgs.AddArgument<float>(stat.Min);
	SetStatBarStatArgs.AddArgument<float>(stat.Max);
	SetStatBarStatArgs.AddArgument<float>(stat.Current);
	m_pManager->m_pElement->CallFunction("SetStatBarStat", SetStatBarStatArgs);
}

void CEntityHudElement_StatBar::SetStat(Stat stat)
{
	m_Stat = stat;

	SUIArguments SetStatBarStatArgs;
	SetStatBarStatArgs.AddArgument<int>(m_Index);
	SetStatBarStatArgs.AddArgument<string>(m_StatName);
	SetStatBarStatArgs.AddArgument<float>(stat.Min);
	SetStatBarStatArgs.AddArgument<float>(stat.Max);
	SetStatBarStatArgs.AddArgument<float>(stat.Current);
	m_pManager->m_pElement->CallFunction("SetStatBarStat", SetStatBarStatArgs);
}

void CEntityHudElement_StatBar::SetStatMin(float Min)
{
	m_Stat.Min = Min;

	SUIArguments SetStatBarMinArgs;
	SetStatBarMinArgs.AddArgument<int>(m_Index);
	SetStatBarMinArgs.AddArgument<float>(Min);
	m_pManager->m_pElement->CallFunction("SetStatBarMin", SetStatBarMinArgs);
}

void CEntityHudElement_StatBar::SetStatMax(float Max)
{
	m_Stat.Max = Max;
	
	SUIArguments SetStatBarMaxArgs;
	SetStatBarMaxArgs.AddArgument<int>(m_Index);
	SetStatBarMaxArgs.AddArgument<float>(Max);
	m_pManager->m_pElement->CallFunction("SetStatBarMin", SetStatBarMaxArgs);
}

void CEntityHudElement_StatBar::SetStat(float stat)
{
	m_Stat.Current = stat;

	SUIArguments SetStatBarValueArgs;
	SetStatBarValueArgs.AddArgument<int>(m_Index);
	SetStatBarValueArgs.AddArgument<float>(stat);
	m_pManager->m_pElement->CallFunction("SetStatBarValue", SetStatBarValueArgs);
}
