#include "StdAfx.h"
#include "DeferredInitialization.h"

std::vector<std::function<void()>> CDeferredInitializationManager::m_DeferredInitializations;
std::unordered_map<UINT_PTR, bool> CDeferredInitializationManager::m_PreInitialized;

void CDeferredInitializationManager::DeferInitialization(std::function<void()> InitFunc)
{
	m_DeferredInitializations.push_back(InitFunc);
}

void CDeferredInitializationManager::TriggerInitializationCheckpoint()
{
	for (int i = 0; i < m_DeferredInitializations.size(); ++i)
	{
		m_DeferredInitializations[i]();
	}
	m_DeferredInitializations.clear();
}

bool CDeferredInitializationManager::IsPreInitialized(UINT_PTR Ptr)
{
	if (m_PreInitialized.find(Ptr) != m_PreInitialized.end())
		return true;
	return false;
}

void CDeferredInitializationManager::PreInitialize(UINT_PTR Ptr)
{
	m_PreInitialized[Ptr] = true;
}
