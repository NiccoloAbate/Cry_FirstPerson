#include "StdAfx.h"
#include "Utils/ConsoleCommands.h"

#include <CryEntitySystem/IEntitySystem.h>

#include "Components/Combat/EncounterSpawnPoint.h"
#include "GamePlugin.h"
#include "Components/Player/Player.h"
#include "Components/Player/PlayerExtension.h"

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
bool is_number(const string& S)
{
	std::string STD = S;
	return is_number(STD);
}

void CC_StartEncounter(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1);
	CC_GETARG(string, Encounter, 1);

	if (IEntity * pSpawnpointEntity = gEnv->pEntitySystem->FindEntityByName("TestEncounter_SP_Player"))
	{
		if (CEncounterSpawnPoint * pEncounterSpawnPoint = pSpawnpointEntity->GetComponent<CEncounterSpawnPoint>())
		{
			pEncounterSpawnPoint->MovePlayerTo();
		}
	}

	if (IEntity * pSpawnpointEntity = gEnv->pEntitySystem->FindEntityByName("TestEncounter_SP_Minion01"))
	{
		if (CEncounterSpawnPoint * pEncounterSpawnpoint = pSpawnpointEntity->GetComponent<CEncounterSpawnPoint>())
		{
			pEncounterSpawnpoint->SpawnAt();
		}
	}

	if (is_number(Encounter))
	{
		int EncounterId;
		FromString(EncounterId, Encounter);
	}
	else
	{
		return;
	}
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("StartEncounter", 1), CC_StartEncounter)


REGISTERDEBUGEXTENSION_WITHFLAGS(CCombatManager, "Combat", EDebugMode::CONSOLE, SHOW_SPAWNPOINTS);
