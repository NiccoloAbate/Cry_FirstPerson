#pragma once

#include "AI\HierarchicalState.h"

#include <DefaultComponents\AI\PathfindingComponent.h>
#include <DefaultComponents\Physics\CharacterControllerComponent.h>

TAGSTATE(HiveAgent, Explore, 2)

class CHiveAgent_AI_Explore : public CAI_HierarchicalState
{
public:

	static constexpr int STATEID = AI::HiveAgent::States::Explore;

	CHiveAgent_AI_Explore(CAI_HierarchicalState *pParentState) : CAI_HierarchicalState(pParentState) {}
	virtual ~CHiveAgent_AI_Explore() {}

	void Initialize();

	FlagType GetDefaultFlagMask() const override { return EAIFlags::NONE; }

protected:

	virtual void OnEnter() override {}
	virtual void OnExit() override {}

	virtual void OnUpdate(float fDeltaTime) override;
	// return true if transitioning
	virtual bool CheckTransitions() override;

	void UpdateMovement(float fDeltaTime);
	void PathfinderCallback(Vec3 RecommendedVelocity);

	IEntity *m_pEntity;

	// Movement
	Cry::DefaultComponents::CCharacterControllerComponent *m_pCharacterController;
	static constexpr float FrontObstacleStopRegion_Length = 2;
	static constexpr float FrontObstacleStopRegion_Width = .8;
	static constexpr float FrontObstacleStopRegion_Height = .8;
	//~Movement
};
