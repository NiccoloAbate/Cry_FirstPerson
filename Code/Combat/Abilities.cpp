#include "StdAfx.h"
#include "Abilities.h"

void CAbility::Activate()
{
	m_Effect();
}
