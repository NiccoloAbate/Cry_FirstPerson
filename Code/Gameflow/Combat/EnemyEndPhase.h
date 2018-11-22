#pragma once

#include "Gameflow\GameflowPhase.h"



TAGGAMEFLOWPHASE(Combat, ENEMY_END, 4)

class CCombatGameflow_EnemyEndPhase : public CGameflowPhase
{
public:

	static constexpr int PHASEID = Gameflow::CombatGameflow::Phases::ENEMY_END;
	static constexpr char* PHASENAME = "Enemy_End";
	CCombatGameflow_EnemyEndPhase() {}

	void Initialize() override;
	void InitializeKeyBinds() override;
	void Update(float fDeltaTime) override;

	FlagType GetDefaultFlagMask() const override { return NULL; }

protected:
};