#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>
#include <vector>

#include <DefaultComponents\Effects\ParticleComponent.h>



class CRewindFrame
{
public:
	CRewindFrame() = default;

	// if == -1 then this is a base frame
	float m_fFrameTime;
	IEntity *m_pEntity = nullptr;

	virtual void CaptureFrame(float fFrameTime) = 0;
	virtual void Rewind(float fFrameTime) = 0;
	virtual CRewindFrame* NextFrame() = 0;

};

////////////////////////////////////////////////////////
// Rewind component
////////////////////////////////////////////////////////
class CRewindComponent : public IEntityComponent
{
public:
	CRewindComponent() : m_Frames(361) {};
	virtual ~CRewindComponent() {}

	static void ReflectType(Schematyc::CTypeDesc<CRewindComponent>& desc)
	{
		desc.SetGUID("{ABDA8FCD-520B-4559-852F-AF22F1803C98}"_cry_guid);
		desc.SetEditorCategory("Rewind");
		desc.SetLabel("Rewind Component");
		desc.SetDescription("Rewind Component Interface");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

#define TARGETREWINDFRAMETIME 0.166666666f // [6 f/sec; 1 / 6]

	void CaptureFrame(float fFrameTime);
	int m_FramesCaptured = 0;
	float m_TimeCaptured = 0;
	std::vector<CRewindFrame*> m_Frames;
	static constexpr int BASEFRAME = 0;
	void SetBaseFrame(CRewindFrame *pBaseFrame) { m_Frames[BASEFRAME] = pBaseFrame; }
	float m_TimeSinceLastCapture = 0;
	void Rewind(float fFrameTime);
	int m_CurrentFrame = -1;

	enum EState
	{
		CAPTURING = 0,
		REWINDING,
		REDOING,
		PAUSED,
	};
	EState m_State = CAPTURING;
	void StartRewinding() { m_CurrentFrame = m_FramesCaptured; m_State = REWINDING; }
	void StartCapturing() { m_State = CAPTURING; }
	void StartRedoing() { m_State = REDOING; }
	void Pause() { m_State = PAUSED; }

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE); };
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent
};

#define REWINDSTATECHECK(pRewindComponent, State)									\
if (pRewindComponent## ->m_State != CRewindComponent::EState:: ##State)				\
	return;																			\




class CRewindFrame_BasicPhysics : public CRewindFrame
{
public:
	CRewindFrame_BasicPhysics() = default;

	Vec3 m_vPos;

	virtual void CaptureFrame(float fFrameTime) override;
	virtual void Rewind(float fFrameTime) override;
	virtual CRewindFrame* NextFrame() override { return new CRewindFrame_BasicPhysics; }

	static CRewindFrame_BasicPhysics* NewFrame() { return new CRewindFrame_BasicPhysics; }

};

class CRewindFrame_PosRot : public CRewindFrame
{
public:
	CRewindFrame_PosRot() = default;

	Vec3 m_vPos;
	Quat m_qRot;

	virtual void CaptureFrame(float fFrameTime) override { m_fFrameTime = fFrameTime; m_vPos = m_pEntity->GetPos(); m_qRot = m_pEntity->GetRotation(); }
	virtual void Rewind(float fFrameTime) override { m_pEntity->SetPos(m_vPos); m_pEntity->SetRotation(m_qRot); }
	virtual CRewindFrame* NextFrame() override { return new CRewindFrame_PosRot; }

	static CRewindFrame_PosRot* NewFrame() { return new CRewindFrame_PosRot; }

};
