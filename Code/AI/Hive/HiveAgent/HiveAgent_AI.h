#pragma once

#include "AI\HierarchicalState.h"

TAGSTATE(HiveAgent, HiveAgent, 0)

class CHiveAgent_AI : public CAI_HierarchicalState
{
public:

	static constexpr int STATEID = AI::HiveAgent::States::HiveAgent;

	CHiveAgent_AI() : CAI_HierarchicalState(nullptr) {}
	virtual ~CHiveAgent_AI() {}

	void Initialize();

	FlagType GetDefaultFlagMask() const override { return EAIFlags::NONE; }

protected:

	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual void OnUpdate(float fDeltaTime);
	// return true if transitioning
	virtual bool CheckTransitions();

};

