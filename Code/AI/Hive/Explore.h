#pragma once

#include "AI\HierarchicalState.h"

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

	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual void OnUpdate(float fDeltaTime);
	// return true if transitioning
	virtual bool CheckTransitions();

};
