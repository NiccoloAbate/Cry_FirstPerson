#pragma once

#include <array>
#include <numeric>

#include "OptimizationManager.h"



class IAutoOptimizer
{
public:

	virtual void Update() = 0;

protected:
	static COptimizationManager* GetOptimizationManager();
	void Register();

	void SetTimer(float Length);
	bool PassTimer();

private:
	class Timer // Length in seconds;
	{
	public:
		Timer(float length) : Length(length) {}

		bool Pass()
		{
			Time += gEnv->pTimer->GetFrameTime();
			return (Time >= Length);
		}
		void Reset() { Time = 0; }
		void Reset(float length) { Length = length; Time = 0; }
		void Destroy() { delete this; }

	private:
		float Length;
		float Time = 0;
	};
	Timer *m_pTimer = nullptr;
};


template <typename T, int nSteps>
class CAutoOptimizer_VariableStep : public IAutoOptimizer
{
public:
	
	// Steps go from low to high; It starts at high (nSteps - 1)
	CAutoOptimizer_VariableStep(T &Var, std::array<T, nSteps> Steps) : m_Var(Var), m_Steps(Steps)
	{
		Register();
		SetTimer(5);
		Var = Steps[m_CurrentStep];
	}

#define STEPDOWN_FRAMERATE 45 // f/s
#define STEPGAP_TIME 15 // seconds

	virtual void Update() override
	{
		if (!PassTimer())
			return;

		COptimizationManager *pManager = GetOptimizationManager();
		if (pManager->IsLoading())
			return;

		if (pManager->GetFrameRate() < STEPDOWN_FRAMERATE)
		{
			StepDown();
		}
	}

protected:

	void StepDown()
	{
		SetTimer(STEPGAP_TIME);
		if (m_CurrentStep != 0)
		{
			m_CurrentStep--;
			m_Var = m_Steps[m_CurrentStep];
		}
	}
	void StepUp()
	{
		SetTimer(STEPGAP_TIME);
		if (m_CurrentStep != nSteps - 1)
		{
			m_CurrentStep++;
			m_Var = m_Steps[m_CurrentStep];
		}
	}

	T &m_Var;
	std::array<T, nSteps> m_Steps;
	int m_CurrentStep = nSteps - 1;
};

#define AUTOOPTIMIZER_VAR_STEP(Type, Name, nSteps, Steps)									\
static Type Name;																			\
static CAutoOptimizer_VariableStep<Type, nSteps> * _p##Name##_AutoOptimizer_ = nullptr;		\
if (!_p##Name##_AutoOptimizer_)																\
_p##Name##_AutoOptimizer_ = new CAutoOptimizer_VariableStep<Type, nSteps>(Name, Steps );	\

#define AUTOOPTIMIZER_LIST(...) {__VA_ARGS__}


