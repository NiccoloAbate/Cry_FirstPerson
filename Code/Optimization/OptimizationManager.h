#pragma once

#include "Types\MovingAverage.h"

class IAutoOptimizer;

class COptimizationManager
{
public:

	void Initialize();
	void Update();

	void StartLoad() { m_bLoading = true; }
	void EndLoad() { m_bLoading = false; }
	bool IsLoading() { return m_bLoading; }

	float GetFrameRate() { return 40; }
	
	void AddAutoOptimizer(IAutoOptimizer *pAutoOptimizer);

protected:
	bool m_bLoading = false;
	MovingAverage<float, 10> m_FrameRate;

	std::vector<IAutoOptimizer*> m_AutoOptimizers;
};
