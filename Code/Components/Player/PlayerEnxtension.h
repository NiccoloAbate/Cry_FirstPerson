#pragma once

#include <CryGame/IGameFramework.h>
#include <IActionMapManager.h>


class CPlayerComponent;

class CPlayerExtension
{
	friend class CPlayerComponent;
protected:
	CPlayerExtension() = default;
	virtual ~CPlayerExtension() {}

	void SetEntity(IEntity *pEntity) { m_pEntity = pEntity; }
	void Extend()
	{
		EnableKeybinds(true);
	}
	void Release()
	{
		EnableKeybinds(false);
	}
	void EnableKeybinds(bool bEnable) { gEnv->pGameFramework->GetIActionMapManager()->EnableActionMap(m_pEntity->GetName(), true); }

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

	const IEntity* GetEntity() { return m_pEntity; }

	virtual void Initialize() = 0;
	//virtual void Update()
	virtual void InitializeKeybinds() = 0;
	virtual void AttachPlayer(IEntity *pPlayerEnt) = 0;

	virtual void OnExtended() = 0;
	virtual void OnReleased() = 0;

protected:
	IEntity *m_pEntity;

};
