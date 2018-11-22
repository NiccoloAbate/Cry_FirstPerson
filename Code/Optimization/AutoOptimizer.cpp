#include "StdAfx.h"
#include "AutoOptimizer.h"

#include "GamePlugin.h"

#include "Utils\ConsoleCommands.h"

COptimizationManager * IAutoOptimizer::GetOptimizationManager()
{
	return CGamePlugin::gGamePlugin->m_pOptimizationManager;
}

void IAutoOptimizer::Register()
{
	GetOptimizationManager()->AddAutoOptimizer(this);
}

void IAutoOptimizer::SetTimer(float Length)
{
	if (!m_pTimer)
		m_pTimer = new Timer(Length);
	else
		m_pTimer->Reset(Length);
}

bool IAutoOptimizer::PassTimer()
{
	if (!m_pTimer)
		return true;
	if (m_pTimer->Pass())
	{
		m_pTimer->Destroy();
		m_pTimer = nullptr;
		return true;
	}
	return false;
}



void CC_TestOptimizer(CC_Args pArgs)
{
	AUTOOPTIMIZER_VAR_STEP(int, Loops, 3, AUTOOPTIMIZER_LIST(1, 2, 3));
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("TestOpt", 0), CC_TestOptimizer)
