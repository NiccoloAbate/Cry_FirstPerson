#pragma once

#include "Utils\Macros\Properties.h"


class CTimelineManager;
class CSerializationEntension;

class CFrameCaptureManager
{
public:

	typedef DynArray<char> FrameBuffer;

	void Initialize(CTimelineManager *pTimelineManager);
	void Update(float fFrameTime);
	
	void StartRewind();
	void EndRewind();
	
	int AddFrameCapturer(CSerializationEntension *pFrameCapturer);

	void Serialize(Serialization::IArchive &ar);

private:
	CTimelineManager *m_pTimelineManager;

	void UpdateCapture(float fFrameTime);
	void UpdateRewind(float fFrameTime);
	void UpdateReplay(float fFrameTime);

	// FrameCapture

	void Capture();
	void Rewind();

	struct FrameInfo
	{
		static float fFrameTime;
		//static std::unordered_map<UINT_PTR, bool> EntityMap;
	};

	std::vector<CSerializationEntension*> m_FrameCapturers;
	float m_TimeSinceLastFrameCapture = 0;

	std::vector<FrameBuffer> m_Frames;
	int m_CurrentFrameIndex = -1;
};
