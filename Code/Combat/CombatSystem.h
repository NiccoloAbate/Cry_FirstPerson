#pragma once

#include "Utils/Debug/DebugExtension.h"
// Add DebugStuff

namespace Combat
{

	enum eUnitTypes
	{
		None = 0,
		Player,
		Minion,
	};

	enum eDebugFlags
	{
		SHOW_SPAWNPOINTS = BIT(1),
	};

}

using namespace Combat;

class CCombatManager
{
public:

	CCombatManager(){}

	DEBUGEXTENSION_WITHFLAGS()

};
