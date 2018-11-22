#pragma once

#include "Types\Flags.h"

#define TAGGAMEFLOW(NAME, Id)											\
namespace Gameflow														\
{																		\
	namespace Gameflows													\
	{																	\
		enum															\
		{																\
			NAME = Id,													\
		};																\
	}																	\
}
TAGGAMEFLOW(NONE, 0)

#define GAMEFLOW_STANDARD_INIT()							\
m_GameflowId = GAMEFLOWID;									\
m_GameflowName = GAMEFLOWNAME;

class CGameflowPhase;

class CGameflow
{
public:

	CGameflow() {}
	virtual ~CGameflow() {}

#define GAMEFLOW_BIT(x) BIT64(x)
	using _FlagNumeric_ = int;
	using FlagType = Flags<_FlagNumeric_>;
	enum EGameflowFlags
	{
		NONE = 0,
		UPDATE_IN_BACKGROUND = GAMEFLOW_BIT(1),
	};

	// Must include GAMEFLOW_STANDARD_INIT()
	virtual void Initialize() = 0;
	// Must include UpdatePhases(fDeltaTime)
	virtual void Update(float fDeltaTime) = 0;

	const int GetGameflowId() { return m_GameflowId; }
	const string GetGameflowName() { return m_GameflowName; }

	virtual FlagType GetDefaultFlagMask() const = 0;
	FlagType& GetFlags() { return m_Flags; }

	enum Phases {};

	CGameflowPhase* GetActivePhase() { return m_Phases[m_ActivePhase]; }
	CGameflowPhase* GetPhase(int Id);

protected:
	int m_GameflowId;
	string m_GameflowName;

	std::vector<CGameflowPhase*> m_Phases;
	int m_ActivePhase;
	void UpdatePhases(float fDeltaTime);
	// Adds an already initialized phase
	int AddPhase(CGameflowPhase *pPhase);

	FlagType m_Flags;
};
