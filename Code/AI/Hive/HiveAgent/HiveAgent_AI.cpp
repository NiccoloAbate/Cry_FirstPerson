#include "StdAfx.h"
#include "HiveAgent_AI.h"

void CHiveAgent_AI::Initialize()
{
	AI_HIERARCHICALSTATE_STANDARD_INIT();
}

void CHiveAgent_AI::OnUpdate(float fDeltaTime)
{
	bool asswhole = true;
}

bool CHiveAgent_AI::CheckTransitions()
{
	return false;
}
