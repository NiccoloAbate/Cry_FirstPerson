#include "StdAfx.h"
#include "Targeting.h"

bool CTargeting::IsValidTarget(IEntity* pTarget)
{
	return true;
}

void CTargeting::Give_BasicTargeting(STargetingInfo_Basic* pInfo)
{
	if (m_pTargetingInfo_Basic) delete m_pTargetingInfo_Basic;
	m_pTargetingInfo_Basic = pInfo;
}

void CTargeting::Give_BasicTargeting(float Range)
{
	Give_BasicTargeting(new STargetingInfo_Basic(Range));
}
