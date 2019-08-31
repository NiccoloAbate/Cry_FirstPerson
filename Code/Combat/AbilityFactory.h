#pragma once


class CAbility;

class CAbilityFactory
{
public:

	enum eAbilities
	{
		TEST = 1,
		SLASH,
	};

	static CAbility* MakeAbility(eAbilities AbilityID);

private:

	static CAbility* MakeAbility_Test();
	static CAbility* MakeAbility_Slash();

};


