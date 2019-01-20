#include "StdAfx.h"
#include "Combat.h"

void CHiveAgent_AI_Combat::Initialize()
{
	AI_HIERARCHICALSTATE_STANDARD_INIT();
}

void CHiveAgent_AI_Combat::OnUpdate(float fDeltaTime)
{
}

bool CHiveAgent_AI_Combat::CheckTransitions()
{
	return false;
}
