#pragma once

#include "AI\HierarchicalState.h"

TAGSTATE(HiveAgent, HiveAgent, 0)

class CHiveAgent_AI : public CAI_HierarchicalState
{
public:

	static constexpr int STATEID = AI::HiveAgent::States::HiveAgent;

	CHiveAgent_AI(IEntity *pHiveEntity) : CAI_HierarchicalState(nullptr), m_pEntity(pHiveEntity) {}
	virtual ~CHiveAgent_AI() {}

	void Initialize();

	FlagType GetDefaultFlagMask() const override { return EAIFlags::NONE; }

	IEntity* GetEntity() const { return m_pEntity; }

protected:

	virtual void OnEnter() override {}
	virtual void OnExit() override {}

	virtual void OnUpdate(float fDeltaTime) override;
	// return true if transitioning
	virtual bool CheckTransitions() override;

	IEntity *m_pEntity;
};

