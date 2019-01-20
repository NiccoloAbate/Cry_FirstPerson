#include "StdAfx.h"
#include "HiveAgent_AI.h"

#include "Explore.h"
#include "Combat.h"

void CHiveAgent_AI::Initialize()
{
	AI_HIERARCHICALSTATE_STANDARD_INIT();

	AddNewSubState<CHiveAgent_AI_Explore>();
	AddNewSubState<CHiveAgent_AI_Combat>();

	SetActiveSubState(AI::HiveAgent::States::Explore);
}

void CHiveAgent_AI::OnUpdate(float fDeltaTime)
{
	bool asswhole = true;
}

bool CHiveAgent_AI::CheckTransitions()
{
	return false;
}
