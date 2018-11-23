#pragma once

#include "Gameflow\GameflowPhase.h"

#include <DefaultComponents\Input\InputComponent.h>


TAGGAMEFLOWPHASE(Combat, PLAYER, 1)

class CCombatGameflow_PlayerPhase : public CGameflowPhase
{
public:
	
	static constexpr int PHASEID = Gameflow::CombatGameflow::Phases::PLAYER;
	static constexpr char* PHASENAME = "Player";
	CCombatGameflow_PlayerPhase() {}

	void Initialize() override;
	void InitializeKeyBinds() override;
	void Update(float fDeltaTime) override;

	FlagType GetDefaultFlagMask() const override { return NULL; }

	void EndPhase();

protected:
	
	void Test();

};


