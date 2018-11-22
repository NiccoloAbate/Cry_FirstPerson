#pragma once

#include "FlashUI\FlashUI.h"
#include "FlashUI\FlashUIAction.h"
#include "FlashUI\FlashUIElement.h"

#include "Types\MinMaxVar.h"

class CEntityHudManager;

class CEntityHudElement
{
public:

	CEntityHudManager *m_pManager;

	int m_Index;
	bool m_bUpdatePos = true;
	bool m_bPosFunc = false;
	Vec3 m_Pos;
	std::function<Vec3()> m_PosFunc;

	virtual void SetVisible(bool Visible) = 0;
	virtual void Update(float fFrameTime) = 0;

	void SetPos(Vec3 Pos) { m_Pos = Pos; }
	void SetPosFun(std::function<Vec3()> PosFunc) { m_PosFunc = PosFunc; }

	void Hide(bool bHidden) { if (bHidden == m_bIsHidden) return; SetVisible(!bHidden); m_bIsHidden = bHidden; }
	bool IsHidden() { return m_bIsHidden; }
	bool IsOnScreen() { return m_bIsOnScreen; }

protected:
	bool m_bIsOnScreen = true;
	bool m_bIsHidden = false;
};

class CEntityHudElement_StatBar : public CEntityHudElement
{
public:
	friend class CEntityHudManager;

	using Stat = MinMaxVar<float>;

	virtual void SetVisible(bool Visible) override;
	virtual void Update(float fFrameTime) override;

	void SetScale(float Scale);

	// Sets m_StatName and m_Stat. Update flash. Don't need to use if the stat is initialized in CEntityHudManager->NewStatBar
	void InitializeStat(string StatName, Stat stat);
	// Sets the whole Stat. Updates flash
	void SetStat(Stat stat);
	void SetStatMin(float Min);
	void SetStatMax(float Max);
	// Sets the current Stat value
	void SetStat(float stat);

	string GetStatName() { return m_StatName; }
	Stat GetStat() { return m_Stat; }

private:
	string m_StatName;
	Stat m_Stat;
};

class CEntityHudManager
{
public:

	IUIElement *m_pElement;

	void Initialize();
	void Update(float fFrameTime);

	CEntityHudElement_StatBar* NewStatBar(string Name, string Type, int X, int Y, string StatName, CEntityHudElement_StatBar::Stat Stat);

	std::vector<CEntityHudElement*> m_pHudElements;
};