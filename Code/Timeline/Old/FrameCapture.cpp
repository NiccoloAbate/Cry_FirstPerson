#include "StdAfx.h"
#include "FrameCapture.h"

#include <CryEntitySystem\IEntity.h>
#include <CryEntitySystem\IEntitySystem.h>

#include "Utils\Serialization\ExtensionFunctions.h"
#include "TimelineManager.h"
#include "GamePlugin.h"

#include "Utils\ConsoleCommands.h"

float CSerializationEntension::FrameInfoBuffer::fFrameTime = -1;

void CSerializationEntension::Serialize(Serialization::IArchive & ar)
{
	
	ar(FrameInfoBuffer::fFrameTime, "frametime");
	for (int i = 0; i < m_SerializationFunctions.size(); i++)
	{
		m_SerializationFunctions[i](ar);
	}

	//SerializationExtension(ar);
}

void CSerializationEntension::AddSerializationExtension(SerializationFunction Function)
{
	m_SerializationFunctions.push_back(Function);
}

void CSerializationEntension::Capture(float fFrameTime)
{
	m_Frames.push_back(FrameBuffer());
	FrameBuffer &NewFrame = m_Frames[m_Frames.size() - 1];

	FrameInfoBuffer::fFrameTime = fFrameTime;
	Serialization::SaveBinaryBuffer(NewFrame, *this);
}

bool CSerializationEntension::Rewind()
{
	if (m_CurrentFrameIndex == -1)
		m_CurrentFrameIndex = m_Frames.size() - 1;
	if (m_CurrentFrameIndex <= 0)
		return false;
	
	FrameBuffer &CurrentFrame = m_Frames[--m_CurrentFrameIndex];
	Serialization::LoadBinaryBuffer(*this, CurrentFrame.data(), CurrentFrame.get_alloc_size());
	return true;
}

void CSerializationEntension::AddEntityTransformFunction(IEntity * pEntity)
{
	AddSerializationExtension(SerializationExtensions::EntityTransform(pEntity));
}

void CSerializationEntension::AddEntityPhysicsFunction(IEntity * pEntity)
{
	AddSerializationExtension(SerializationExtensions::EntityPhysics(pEntity));
}

void CSerializationEntension::Register()
{
	CGamePlugin::gGamePlugin->m_pTimelineManager->AddFrameCapturer(this);
}



void CC_FCTest(CC_Args pArgs)
{

	IEntity *pEntity = gEnv->pEntitySystem->GetEntityIterator()->Next();

	CSerializationEntension *pFrameCapturer = new CSerializationEntension;
	pFrameCapturer->AddEntityTransformFunction(pEntity);
	pFrameCapturer->Capture(0);
	pFrameCapturer->Capture(1);

	pFrameCapturer->Rewind();

}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("FCTest", 0), CC_FCTest)

