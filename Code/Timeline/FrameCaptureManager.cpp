#include "StdAfx.h"
#include "FrameCaptureManager.h"

#include <CryEntitySystem\IEntity.h>
#include <CryEntitySystem\IEntitySystem.h>

#include "TimelineManager.h"
#include "Utils\Serialization\SerializationEnxtension.h"

#include "Optimization\AutoOptimizer.h"


float CFrameCaptureManager::FrameInfo::fFrameTime = 0;
//std::unordered_map<UINT_PTR, bool> CFrameCaptureManager::FrameInfo::EntityMap = std::unordered_map<UINT_PTR, bool>();

void CFrameCaptureManager::Initialize(CTimelineManager * pTimelineManager)
{
	m_pTimelineManager = pTimelineManager;
}

void CFrameCaptureManager::Update(float fFrameTime)
{

	switch (m_pTimelineManager->GetState())
	{
	case CTimelineManager::EState::CAPTURING:
		UpdateCapture(fFrameTime);
		break;
	case CTimelineManager::EState::REWINDING:
		UpdateRewind(fFrameTime);
		break;
	case CTimelineManager::EState::REPLAYING:
		UpdateReplay(fFrameTime);
		break;
	default:
		break;
	}

}

void CFrameCaptureManager::StartRewind()
{
}

void CFrameCaptureManager::EndRewind()
{
}

int CFrameCaptureManager::AddFrameCapturer(CSerializationEntension * pFrameCapturer)
{
	//UINT_PTR Ptr = UINT_PTR(pFrameCapturer);
	//if (FrameInfo::EntityMap.find(Ptr) == FrameInfo::EntityMap.end())
	//	FrameInfo::EntityMap[Ptr] = true;
	//else
	//	return -1;
	m_FrameCapturers.push_back(pFrameCapturer);
	return m_FrameCapturers.size() - 1;
}

void CFrameCaptureManager::Serialize(Serialization::IArchive & ar)
{

	ar(FrameInfo::fFrameTime, "fFrameTime");
	//ar(FrameInfo::EntityMap, "EntityMap");
	for (int i = 0; i < m_FrameCapturers.size(); i++)
	{
		//if (FrameInfo::EntityMap.find(UINT_PTR(m_FrameCapturers[i])) == FrameInfo::EntityMap.end())
		//	continue;
		m_FrameCapturers[i]->Serialize(ar);
	}

}

void CFrameCaptureManager::UpdateCapture(float fFrameTime)
{
	AUTOOPTIMIZER_VAR_STEP(float, TIMEBETWEENFRAMECAPTURES, 3, AUTOOPTIMIZER_LIST(.2, .15, .1));

	m_TimeSinceLastFrameCapture += fFrameTime;
	if (m_TimeSinceLastFrameCapture >= TIMEBETWEENFRAMECAPTURES)
	{
		Capture();
		m_TimeSinceLastFrameCapture = 0;
	}
}

void CFrameCaptureManager::UpdateRewind(float fFrameTime)
{
	if (m_CurrentFrameIndex == 0)
	{
		m_pTimelineManager->EndRewind();
		return;
	}
	Rewind();
}

void CFrameCaptureManager::UpdateReplay(float fFrameTime)
{
}

void CFrameCaptureManager::Capture()
{
	m_CurrentFrameIndex++;

	m_Frames.push_back(FrameBuffer());
	FrameBuffer &NewFrame = m_Frames[m_CurrentFrameIndex];

	FrameInfo::fFrameTime = m_TimeSinceLastFrameCapture;
	Serialization::SaveBinaryBuffer(NewFrame, *this);
}

void CFrameCaptureManager::Rewind()
{
	FrameBuffer &CurrentFrame = m_Frames[--m_CurrentFrameIndex];
	Serialization::LoadBinaryBuffer(*this, CurrentFrame.data(), CurrentFrame.get_alloc_size());

	gEnv->pLog->Log(ToString(m_CurrentFrameIndex) + "   " + ToString(gEnv->pTimer->GetFrameTime()));
}
