#pragma once

struct STargetingInfo_Basic
{
	STargetingInfo_Basic() {}
	STargetingInfo_Basic(float range) : Range(range) {}

	float Range;
};


class CTargeting
{
public:

	CTargeting() {}
	virtual ~CTargeting() { DeleteInfo(); }

	bool IsValidTarget(IEntity* pTarget);

	void Give_BasicTargeting(STargetingInfo_Basic* pInfo);
	void Give_BasicTargeting(float Range);
	bool Has_BasicTargeting() { return m_pTargetingInfo_Basic != nullptr; }
	STargetingInfo_Basic* Get_BasicTargeting() { return m_pTargetingInfo_Basic; }

	void SetTargetEntity(IEntity* pEntity) { m_pTargetEnt = pEntity; }
	IEntity* GetTargetEntity() const { return m_pTargetEnt; }
	void SetUserEntity(IEntity* pEntity) { m_pUserEnt = pEntity; }
	IEntity* GetUserEntity() const { return m_pUserEnt; }


protected:

	void DeleteInfo()
	{
		if (m_pTargetingInfo_Basic) delete m_pTargetingInfo_Basic;
	}
	STargetingInfo_Basic* m_pTargetingInfo_Basic = nullptr;

	IEntity* m_pUserEnt = nullptr;
	IEntity* m_pTargetEnt = nullptr;

};


