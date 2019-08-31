#pragma once

#include <CryEntitySystem/IEntity.h>
#include "Targeting.h"

#include <functional>

struct SAbilityInfo
{
	enum eAbilityType
	{
		Active = 0,
		Passive,
	};

	SAbilityInfo() = default;
	SAbilityInfo(string name, eAbilityType type = Active) : Name(name), Type(type) {}

	string Name;
	eAbilityType Type = Active;
};

class CAbility
{
public:

	friend class CAbilityFactory;

	CAbility() = default;
	CAbility(SAbilityInfo Info) : m_AbilityInfo(Info) {}
	virtual ~CAbility() {}

	SAbilityInfo& GetAbilityInfo() { return m_AbilityInfo; }
	string GetName() { return m_AbilityInfo.Name; }
	void SetName(string Name) { m_AbilityInfo.Name = Name; }
	SAbilityInfo::eAbilityType GetType() { return m_AbilityInfo.Type; }
	void SetType(SAbilityInfo::eAbilityType Type) { m_AbilityInfo.Type = Type; }

	CTargeting& GetTargeting() { return m_Targeting; }

	void Activate();

protected:

	SAbilityInfo m_AbilityInfo;
	CTargeting m_Targeting;
	
	std::function<void()> m_Effect;

};



