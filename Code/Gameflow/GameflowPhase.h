#pragma once

#include <DefaultComponents\Input\InputComponent.h>

#include "Types\Flags.h"

/*////////////////////////////////////////////////////////////////
When making GameflowPhase:

TAGGAMEFLOWPHASE(Combat, PLAYER, 1)

static constexpr int PHASEID = Gameflow::CombatGameflow::Phases::PLAYER;
static constexpr char* PHASENAME = "Player";



*/////////////////////////////////////////////////////////////////


#define TAGGAMEFLOWPHASE(GameflowName, PHASENAME, Id)			\
namespace Gameflow												\
{																\
	namespace GameflowName##Gameflow							\
	{															\
		namespace Phases										\
		{														\
			enum												\
			{													\
				PHASENAME = Id,									\
			};													\
		}														\
	}															\
}

#define GAMEFLOWPHASE_STANDARD_INIT()							\
m_PhaseId = PHASEID;											\
m_PhaseName = PHASENAME;										\
m_Flags = GetDefaultFlagMask();									\
InitializeKeyBinds();

#define ACTIVEPHASECHECK() if(!IsActivePhase()) return;
#define GAMEFLOWPHASE_ACTION_STARTCHECK() if(!(IsActivePhase() || GetFlags().HasFlag(EGameflowPhaseFlags::UPDATE_IN_BACKGROUND))) return;

class CGameflow;
class CGameflowManager;

class CGameflowPhase
{
	friend class CGameflow;
public:

#define GAMEFLOWPHASE_BIT(x) BIT64(x)
	using _FlagNumeric_ = int;
	using FlagType = CFlags<_FlagNumeric_>;
	enum EGameflowPhaseFlags
	{
		NONE = 0,
		UPDATE_IN_BACKGROUND = GAMEFLOWPHASE_BIT(1),
	};
	
	// Must include GAMEFLOWPHASE_STANDARD_INIT();
	virtual void Initialize() = 0;
	// pInputComponent is optional; if not set, the active player input component will be used
	virtual void InitializeKeyBinds() = 0;
	virtual void Update(float fDeltaTime) = 0;

	virtual void OnPhaseStart() {}
	virtual void OnPhaseEnd() {}

	const int GetPhaseId() { return m_PhaseId; }
	const string GetPhaseName() { return m_PhaseName; }

	virtual FlagType GetDefaultFlagMask() const = 0;
	FlagType& GetFlags() { return m_Flags; }

	CGameflowManager* GetGameflowManager();
	CGameflow* GetParentGameflow() { return m_pParentGameflow; }
	bool IsActivePhase();


protected:
	int m_PhaseId;
	string m_PhaseName;

	void SetParentGameflow(CGameflow *pParent) { m_pParentGameflow = pParent; }
	CGameflow *m_pParentGameflow = nullptr;

	FlagType m_Flags;
	
	Cry::DefaultComponents::CInputComponent *m_pInputComponent;
	Cry::DefaultComponents::CInputComponent* GetInputComponent() { return m_pInputComponent; }

	void SetActivePhase(bool bActive);

	static Cry::DefaultComponents::CInputComponent* GetPlayerInputComponent();

};


