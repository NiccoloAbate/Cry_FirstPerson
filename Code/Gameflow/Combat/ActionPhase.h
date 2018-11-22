#pragma once

#include "Gameflow\GameflowPhase.h"

#include <DefaultComponents\Input\InputComponent.h>


TAGGAMEFLOWPHASE(Combat, ACTION, 1)

class CCombatGameflow_ActionPhase : public CGameflowPhase
{
public:
	
	static constexpr int PHASEID = Gameflow::CombatGameflow::Phases::ACTION;
	static constexpr char* PHASENAME = "Combat";
	CCombatGameflow_ActionPhase() {}

	void Initialize() override;
	void InitializeKeyBinds(Cry::DefaultComponents::CInputComponent *pInputComponent = nullptr);
	void Update(float fDeltaTime) override;

	FlagType GetDefaultFlagMask() const override { return NULL; }

protected:

	void Test();

};


