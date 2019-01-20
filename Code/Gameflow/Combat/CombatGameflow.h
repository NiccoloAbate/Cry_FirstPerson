#pragma once

#include "Gameflow\Gameflow.h"

TAGGAMEFLOW(COMBAT, 1)

class CCharacterComponent;

class CCombatGameflow : public CGameflow
{
public:

	static constexpr int GAMEFLOWID = Gameflow::Gameflows::COMBAT;
	static constexpr char* GAMEFLOWNAME = "Combat";
	CCombatGameflow() {}

	CCharacterComponent* GetCharacter() const { return m_pCharacter; }
	bool IsPlayerOutOfMoves();

	virtual void Initialize() override;
	virtual void InitializeKeyBinds() override;
	virtual void Update(float fDeltaTime) override;

	virtual FlagType GetDefaultFlagMask() const override { return NULL; }


protected:

	CCharacterComponent *m_pCharacter;

};

