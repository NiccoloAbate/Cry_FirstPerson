#pragma once

#include "FlashUI\FlashUI.h"
#include "FlashUI\FlashUIAction.h"
#include "FlashUI\FlashUIElement.h"

class CSpeechBubbleManager;

class CSpeechBubble
{
public:

	CSpeechBubbleManager *m_pManager;

	int m_Index;
	float m_Life = -1;
	bool m_bUpdatePos = true;
	bool m_bPosFunc = false;
	Vec3 m_Pos;
	std::function<Vec3()> m_PosFunc;

	void Update(float fFrameTime);
	void Destroy();
	void SetPos(Vec3 Pos) { m_Pos = Pos; m_bPosFunc = false;}
	void SetPosFunc(std::function<Vec3()> PosFunc) { m_PosFunc = PosFunc; m_bPosFunc = true; }
	void SetText(string Text);
	
};

class CSpeechBubbleManager
{
public:

	IUIElement *m_pElement;

	void Initialize();
	void Update(float fFrameTime);

	CSpeechBubble* NewBubble(string Text, float Life = -1);

	std::vector<CSpeechBubble*> m_pSpeechBubbles;
};
