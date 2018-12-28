#pragma once

#include "Gameflow\GameflowPhase.h"



TAGGAMEFLOWPHASE(Combat, PLAYER_END, 2)

class CCombatGameflow_PlayerEndPhase : public CGameflowPhase
{
public:

	static constexpr int PHASEID = Gameflow::CombatGameflow::Phases::PLAYER_END;
	static constexpr char* PHASENAME = "Player_End";
	CCombatGameflow_PlayerEndPhase() {}

	void Initialize() override;
	void OnPhaseStart() override;
	void InitializeKeyBinds() override;
	void Update(float fDeltaTime) override;

	FlagType GetDefaultFlagMask() const override { return NULL; }

protected:
};
