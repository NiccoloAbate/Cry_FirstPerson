#pragma once

#include "AI\HierarchicalState.h"



TAGSTATE(HiveAgent, Combat, 1)

class CHiveAgent_AI_Combat : public CAI_HierarchicalState
{
public:

	static constexpr int STATEID = AI::HiveAgent::States::Combat;

	CHiveAgent_AI_Combat(CAI_HierarchicalState *pParentState) : CAI_HierarchicalState(pParentState) {}
	virtual ~CHiveAgent_AI_Combat() {}

	void Initialize();

	FlagType GetDefaultFlagMask() const override { return EAIFlags::NONE; }

protected:

	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual void OnUpdate(float fDeltaTime);
	// return true if transitioning
	virtual bool CheckTransitions();

};

