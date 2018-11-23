#pragma once

#include <CryGame/IGameFramework.h>
#include <IActionMapManager.h>


class CPlayerComponent;

class CPlayerExtension
{
	friend class CPlayerComponent;
protected:
	CPlayerExtension();
	virtual ~CPlayerExtension() {}

public:

	template <typename T, typename = enable_if<is_base_of<CPlayerExtension, T>::value>::type>
	static T* Make(IEntity *pParentEnt)
	{
		T *pExtension = new T;
		pExtension->SetEntity(pParentEnt);
		pExtension->Initialize();
		pExtension->InitializeKeyBinds();
		return pExtension;
	}

	const CPlayerComponent* GetPlayerComponent() { return m_pPlayerComponent; }

	virtual void Initialize() = 0;
	//virtual void Update()
	virtual void ProcessKeyEvent(EKeyId KeyId, int activatioMode, float value) = 0;
	virtual void AttachPlayer(IEntity *pPlayerEnt) = 0;

	virtual void Update(float fFramTime) = 0;

	virtual void OnExtended() {}
	virtual void OnReleased() {}

protected:
	CPlayerComponent *m_pPlayerComponent;

};
