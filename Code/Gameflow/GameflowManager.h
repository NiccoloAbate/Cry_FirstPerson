#pragma once

#include "Utils\Debug\DebugExtension.h"

#include <CryEntitySystem\IEntity.h>
#include <DefaultComponents\Input\InputComponent.h>


class CGameflow;

class CGameflowManager
{
public:

	void Initialize();
	void Update(float fDeltaTime);

	CGameflow* GetActiveGameflow() { return m_Gameflows[m_ActiveGameflow]; }
	// Gets the first found gameflow with given Id
	CGameflow* GetGameflowFromId(int Id);

	Cry::DefaultComponents::CInputComponent* GetInputComponent() { return m_pInputComponent; }

	DEBUGEXTENSION()

private:

	void InitializeGameflows();
	std::vector<CGameflow*> m_Gameflows;
	int m_ActiveGameflow;
	// Adds an already initialized gameflow
	int AddGameflow(CGameflow *pGameflow);
	// Sets by the index of m_Gameflows (*NOT THE GAMEFLOW ID*)
	void SetActiveGameflowIndex(int Index);
	// Sets by gameflow Id
	void SetActiveGameflow(int Id);
	void SetActiveGameflow(CGameflow *pGameflow);
	void SetActiveGameflow_Debug(CGameflow *pGameflow);

	Cry::DefaultComponents::CInputComponent *m_pInputComponent = nullptr;

};
