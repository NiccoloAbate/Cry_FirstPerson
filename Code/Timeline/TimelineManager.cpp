#include "StdAfx.h"
#include "TimelineManager.h"

#include "Timeline\FrameCaptureManager.h"
#include "Optimization\AutoOptimizer.h"

#include "Utils\ConsoleCommands.h"
#include "GamePlugin.h"

void CTimelineManager::Initialize()
{
	m_pFrameCaptureManager = new CFrameCaptureManager;
	m_pFrameCaptureManager->Initialize(this);
}

void CTimelineManager::Update(float fFrameTime)
{
	if (m_GameTimeScale == 0)
		return;

	m_pFrameCaptureManager->Update(fFrameTime);
	
}

void CTimelineManager::StartRewind()
{
	SetState(REWINDING);
	SetGameTimeScale(-1);
	SetCryTimeScale(1);
	
	m_pFrameCaptureManager->StartRewind();
}

void CTimelineManager::EndRewind()
{
	SetState(CAPTURING);
	SetTimeScale(0);

	m_pFrameCaptureManager->EndRewind();
}

void CTimelineManager::SetTimeScale(float Scale)
{
	SetGameTimeScale(Scale);
	SetCryTimeScale(Scale);
}

void CTimelineManager::SetGameTimeScale(float Scale)
{
	m_GameTimeScale = Scale;
}

void CTimelineManager::SetCryTimeScale(float Scale)
{
	if (Scale < CRYTIMESCALE_MIN)
		m_CryTimeScale = CRYTIMESCALE_MIN;
	else
		m_CryTimeScale = Scale;
	gEnv->pConsole->ExecuteString("t_scale " + ToString(m_CryTimeScale));
}



void CC_TestRewind(CC_Args pArgs)
{
	CGamePlugin::gGamePlugin->m_pTimelineManager->StartRewind();
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("TestRewind", 0), CC_TestRewind)
