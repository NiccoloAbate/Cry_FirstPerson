#pragma once

#include <functional>
#include <unordered_map>

#define DEFER_THISINITIALIZATION() CDeferredInitializationManager::DeferInitialization([this]() {this->Initialize(); });	return;
#define TRIGGER_DEFERREDINITIALIZATION_CHECKPOINT() CDeferredInitializationManager::TriggerInitializationCheckpoint();

#define PREINITIALIZE(pPointer) pPointer##->Initialize(); CDeferredInitializationManager::PreInitialize((UINT_PTR)pPointer);
#define IS_THIS_PREINITIALIZED() CDeferredInitializationManager::IsPreInitialized((UINT_PTR)this)

class CDeferredInitializationManager
{
public:

	static void DeferInitialization(std::function<void()> InitFunc);
	static void TriggerInitializationCheckpoint();

	//Used to say when something is PREinitialized (i. e. a later initialization will be cancelled)
	static void PreInitialize(UINT_PTR Ptr);
	static bool IsPreInitialized(UINT_PTR Ptr);

private:
	static std::vector<std::function<void()>> m_DeferredInitializations;
	static std::unordered_map<UINT_PTR, bool> m_PreInitialized;
};
