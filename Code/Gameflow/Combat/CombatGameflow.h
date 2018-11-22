#pragma once

#include "Gameflow\Gameflow.h"

TAGGAMEFLOW(COMBAT, 1)

class CCombatGameflow : public CGameflow
{
public:

	static constexpr int GAMEFLOWID = Gameflow::Gameflows::COMBAT;
	static constexpr char* GAMEFLOWNAME = "Combat";
	CCombatGameflow() {}





	virtual void Initialize() override;
	virtual void Update(float fDeltaTime) override;

	virtual FlagType GetDefaultFlagMask() const override { return NULL; }


	

};

