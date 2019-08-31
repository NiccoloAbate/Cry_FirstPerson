#include "StdAfx.h"
#include "AbilityFactory.h"
#include "Abilities.h"

#include "Components/Game/GameplayEntity.h"

CAbility* CAbilityFactory::MakeAbility(eAbilities AbilityID)
{
	CAbility* pAbility = nullptr;

	switch (AbilityID)
	{
	case eAbilities::TEST:
		pAbility = MakeAbility_Test();
		break;
	case eAbilities::SLASH:
		pAbility = MakeAbility_Slash();
		break;
	default:
		break;
	}

	return pAbility;
}

CAbility* CAbilityFactory::MakeAbility_Test()
{
	CAbility* pAbility = new CAbility(SAbilityInfo("Test"));
	pAbility->m_Targeting.Give_BasicTargeting(20);
	pAbility->m_Effect = [pAbility]() 
		{
			IEntity* pTargetEnt = pAbility->GetTargeting().GetTargetEntity();
			if (!pTargetEnt)
				return;
			string TargetName = pTargetEnt->GetName();
			gEnv->pLog->Log("Test Ability -> " + TargetName);

			IEntity* pUserEnt = pAbility->GetTargeting().GetUserEntity();
			if (!pUserEnt)
				return;

			const float Strength = 10000;
			pe_action_impulse Impulse;
			Impulse.impulse = pUserEnt->GetForwardDir() * Strength;
			pTargetEnt->GetPhysicalEntity()->Action(&Impulse);
		};
	return pAbility;
}

CAbility* CAbilityFactory::MakeAbility_Slash()
{
	CAbility* pAbility = new CAbility(SAbilityInfo("Slash"));
	pAbility->m_Targeting.Give_BasicTargeting(5);
	pAbility->m_Effect = [pAbility]()
	{
		IEntity* pTargetEnt = pAbility->GetTargeting().GetTargetEntity();
		if (!pTargetEnt)
			return;
		string TargetName = pTargetEnt->GetName();
		gEnv->pLog->Log("Slash -> " + TargetName);

		const float Damage = 3;

		if (CGameplayEntityComponent * pGameplayEnt = pTargetEnt->GetComponent<CGameplayEntityComponent>())
		{
			gEnv->pLog->Log(ToString(pGameplayEnt->GetHealth()));
			pGameplayEnt->GetHealth() -= Damage;
			gEnv->pLog->Log(ToString(pGameplayEnt->GetHealth()));
		}
	};
	return pAbility;
}
