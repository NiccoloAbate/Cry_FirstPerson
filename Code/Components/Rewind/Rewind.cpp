#include "StdAfx.h"
#include "Rewind.h"

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CryEntitySystem\IEntity.h>

#include "Utils\ConsoleCommands.h"

static void RegisterRewindComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CRewindComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterRewindComponent)

void CRewindComponent::CaptureFrame(float fFrameTime)
{
	if (m_FramesCaptured == m_Frames.size() - 1)
		return;
	CRewindFrame *pNewFrame = m_Frames[BASEFRAME]->NextFrame();
	pNewFrame->m_pEntity = m_pEntity;
	pNewFrame->CaptureFrame(fFrameTime);
	m_FramesCaptured++;
	m_Frames[m_FramesCaptured] = pNewFrame;
}

void CRewindComponent::Rewind(float fFrameTime)
{
	if (m_CurrentFrame == BASEFRAME + 1)
		return;
	m_CurrentFrame--;
	m_Frames[m_CurrentFrame]->Rewind(fFrameTime);
}

void CRewindComponent::Initialize()
{
}

void CRewindComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	default:
		break;
	case ENTITY_EVENT_UPDATE:
	{

		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		switch (m_State)
		{
		default:
			break;
		case CAPTURING:
			m_TimeSinceLastCapture += pCtx->fFrameTime;
			if (m_TimeSinceLastCapture >= TARGETREWINDFRAMETIME)
			{
				CaptureFrame(m_TimeSinceLastCapture);
				m_TimeCaptured += m_TimeSinceLastCapture;
				m_TimeSinceLastCapture = 0;
			}
			break;
		case REWINDING:
			Rewind(pCtx->fFrameTime);
			break;
		case REDOING:
			break;
		case PAUSED:
			break;
		}

	}
	}
}



void CRewindFrame_BasicPhysics::CaptureFrame(float fFrameTime)
{
	m_fFrameTime = fFrameTime;
	if (!m_pEntity)
		return;
	m_vPos = m_pEntity->GetPos();
}

void CRewindFrame_BasicPhysics::Rewind(float fFrameTime)
{
	m_pEntity->SetPos(m_vPos);
}
