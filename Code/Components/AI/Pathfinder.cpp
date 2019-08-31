#include "StdAfx.h"
#include "Pathfinder.h"

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

static void RegisterPathfinderComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPathfinderComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPathfinderComponent)

void CPathfinderComponent::Initialize()
{
	m_PathfindingMethod = STRAIGHTLINE;
	m_PathindingFlags.AddFlags(GROUNDED);
}

void CPathfinderComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_UPDATE:
		Update();
		break;
	default:
		break;
	}
}

void CPathfinderComponent::Update()
{
	if (!m_bHasRequest)
		return;
	if ((m_CurrentRequestedDestination - m_pEntity->GetWorldPos()).GetLengthFast() <= m_MovementParams.EndRadius)
	{
		if (m_DestinationCallback)
			m_DestinationCallback();
		CancelCurrentRequest();
		return;
	}

	Vec3 RecommendedVelocity;
	switch (m_PathfindingMethod)
	{
	case STRAIGHTLINE:
		Update_StraightLine(RecommendedVelocity);
		break;
	default:
		break;
	}
	
	if (m_PathindingFlags.HasFlag(GROUNDED)) // Gets rid of vertical velocity while retaining same overral speed
	{
		float Speed = RecommendedVelocity.GetLengthFast();
		RecommendedVelocity.z = 0;
		RecommendedVelocity = RecommendedVelocity.GetNormalized() * Speed;
	}

	if (!m_MovementCallback)
	{
		const string Message = "There is no pathinding movement callback!";
		if (m_DebugMode >= EDebugMode::CONSOLE)
		{
			gEnv->pLog->LogError(Message);
		}
		return;
	}

	m_MovementCallback(RecommendedVelocity);
}

void CPathfinderComponent::Update_StraightLine(Vec3 & RecommendedVelocity)
{
	Vec3 Dir = m_CurrentRequestedDestination - m_pEntity->GetWorldPos();
	RecommendedVelocity = Dir.GetNormalized() * m_MovementParams.MoveSpeed;
}

void CPathfinderComponent::RequestMoveTo_Debug(Vec3 Destination)
{
	if (m_DebugMode == EDebugMode::NONE)
		return;
	const string Message = "Pathfinding MoveToReqeust to " + ToString(Destination.x) + ", " + ToString(Destination.y) + ", " + ToString(Destination.z);
	if (m_DebugMode >= EDebugMode::CONSOLE)
	{
		gEnv->pLog->Log(Message);
	}
}

REGISTERDEBUGEXTENSION(CPathfinderComponent, "Pathfinding", EDebugMode::CONSOLE);
