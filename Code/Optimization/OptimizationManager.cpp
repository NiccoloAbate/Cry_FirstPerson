#include "StdAfx.h"
#include "OptimizationManager.h"

#include "AutoOptimizer.h"

void COptimizationManager::Initialize()
{
}

void COptimizationManager::Update()
{
	float FrameRate = gEnv->pTimer->GetFrameRate();
	m_FrameRate.Push(FrameRate);

	for (int i = 0; i < m_AutoOptimizers.size(); i++)
	{
		m_AutoOptimizers[i]->Update();
	}

}

void COptimizationManager::AddAutoOptimizer(IAutoOptimizer * pAutoOptimizer)
{
	m_AutoOptimizers.push_back(pAutoOptimizer);
}
