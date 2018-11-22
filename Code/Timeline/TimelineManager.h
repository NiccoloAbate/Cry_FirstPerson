#pragma once

#include "FrameCaptureManager.h"

#include "Utils\Macros\Properties.h"


class CFrameCaptureManager;

class CTimelineManager
{
public:

	void Initialize();
	void Update(float fFrameTime);

	CFrameCaptureManager *m_pFrameCaptureManager;

	enum EState
	{
		CAPTURING = 0,
		REWINDING,
		REPLAYING,
	};
	PROPERTY_DEFAULTVAL(EState, State, CAPTURING)
	bool IsInState(EState State) { return (State == m_State); }

	void StartRewind();
	void EndRewind();

	void SetTimeScale(float Scale);
	void SetGameTimeScale(float Scale);
	float& GetGameTimeScale() { return m_GameTimeScale; }
	void SetCryTimeScale(float Scale);
	float& GetCryTimeScale() { return m_CryTimeScale; }
	
private:

	float m_GameTimeScale = 1;
	float m_CryTimeScale = 1;
#define CRYTIMESCALE_MIN .075f

};
