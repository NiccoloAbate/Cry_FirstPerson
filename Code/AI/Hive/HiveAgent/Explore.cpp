#include "StdAfx.h"
#include "Explore.h"
#include "HiveAgent_AI.h"

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents\AI\PathfindingComponent.h>
#include "Components\AI\Pathfinder.h"

#include "Types\MovingAverage.h"
#include "Types\MinMaxVar.h"

void CHiveAgent_AI_Explore::Initialize()
{
	AI_HIERARCHICALSTATE_STANDARD_INIT();

	m_pEntity = ((CHiveAgent_AI*)m_pParentState)->GetEntity();
	if (CPathfinderComponent *pPathfinderComponent = m_pEntity->GetComponent<CPathfinderComponent>())
	{
		pPathfinderComponent->SetMovementCallback([this](const Vec3& recommendedVelocity)
		{
			PathfinderCallback(recommendedVelocity);
		});
	}
	m_pCharacterController = m_pEntity->GetComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
}

void CHiveAgent_AI_Explore::OnUpdate(float fDeltaTime)
{
	bool dankkush = true;
}

bool CHiveAgent_AI_Explore::CheckTransitions()
{
	return false;
}

void CHiveAgent_AI_Explore::UpdateMovement(float fDeltaTime)
{

}

void CHiveAgent_AI_Explore::PathfinderCallback(Vec3 RecommendedVelocity)
{
	if (!m_pCharacterController)
		return;
	AABB BoundingBox;
	m_pEntity->GetLocalBounds(BoundingBox);
	const Vec3 EntPos = m_pEntity->GetWorldPos() + (BoundingBox.GetCenter() - Vec3(0, 0, BoundingBox.GetSize().z / 2));
	const Vec3 WidthOffset = RecommendedVelocity.GetRotated({ 0, 0, 1 }, 90).GetNormalized() * (FrontObstacleStopRegion_Width / 2);
	const Vec3 BBMin = EntPos - WidthOffset + Vec3(0, 0, .1);
	const Vec3 LengthOffset = RecommendedVelocity * FrontObstacleStopRegion_Length;
	const Vec3 BBMax = EntPos + WidthOffset + LengthOffset + Vec3(0, 0, FrontObstacleStopRegion_Height) + Vec3(0, 0, .1);
	
	IPhysicalEntity **pPhysList;
	const int NumEnts = gEnv->pPhysicalWorld->GetEntitiesInBox(BBMin, BBMax, pPhysList, ent_all);
	
	if (NumEnts > 1) // 1 because itself is in the box
	{
		m_pCharacterController->SetVelocity(RecommendedVelocity.GetRotated({ 0, 0, 1 }, 80).GetNormalized() * RecommendedVelocity.GetLength());
	}
	else
	{
		m_pCharacterController->SetVelocity(RecommendedVelocity);
	}
}
