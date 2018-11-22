#pragma once

#include "Gameflow\GameflowPhase.h"

TAGGAMEFLOWPHASE(Combat, ENEMY, 3)

class CCombatGameflow_EnemyPhase : public CGameflowPhase
{
public:

	static constexpr int PHASEID = Gameflow::CombatGameflow::Phases::ENEMY;
	static constexpr char* PHASENAME = "Enemy";
	CCombatGameflow_EnemyPhase() {}

	void Initialize() override;
	void InitializeKeyBinds() override;
	void Update(float fDeltaTime) override;

	FlagType GetDefaultFlagMask() const override { return NULL; }

protected:
};
