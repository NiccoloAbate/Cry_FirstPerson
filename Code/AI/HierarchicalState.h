#pragma once

#include <vector>

#include "AI\AI.h"

#define TAGSTATE(AINAME, STATENAME, Id)									\
namespace AI															\
{																		\
	namespace AINAME													\
	{																	\
		namespace States												\
		{																\
			enum														\
			{															\
				STATENAME = Id,											\
			};															\
		}																\
	}																	\
}

#define AI_HIERARCHICALSTATE_STANDARD_INIT()							\
m_StateId = STATEID;

class CAI_HierarchicalState : public CAI
{
public:

	CAI_HierarchicalState(CAI_HierarchicalState *pParentState) : m_pParentState(pParentState) {}
	virtual ~CAI_HierarchicalState() {}

	// Must include AI_HIERARCHICALSTATE_STANDARD_INIT()
	virtual void Initialize() override = 0;
	void Update(float fDeltaTime) override;

	virtual FlagType GetDefaultFlagMask() const override = 0;

	const int GetStateId() { return m_StateId; }
	const CAI_HierarchicalState* GetParentState() { return m_pParentState; }

protected:
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void OnUpdate(float fDeltaTime) = 0;
	// return true if transitioning
	virtual bool CheckTransitions() = 0;

	CAI_HierarchicalState *m_pParentState;
	// Unique within every entire state machine
	int m_StateId;

	std::vector<CAI_HierarchicalState*> m_SubStates;
	int m_ActiveSubState = -1;

	void TransitionTo(const int StateId);

};
