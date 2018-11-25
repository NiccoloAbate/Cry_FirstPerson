#pragma once

#include "CryInput\IInput.h"

#include "Types\Flags.h"

class CPlayerComponent;
class CHud;

class CPlayerExtension
{
	friend class CPlayerComponent;
protected:
	CPlayerExtension(IEntity *pEntity) : m_pEntity(pEntity) {}
	virtual ~CPlayerExtension() {}

	void Release();
	void ExtendFrom(CPlayerComponent *pPlayerComponent);

public:

	IEntity* GetEntity() { return m_pEntity; }

	const bool IsExtended() { return m_bExtended; }
	const CPlayerComponent* GetPlayerComponent() { return m_pPlayerComponent; }

	virtual void Initialize() = 0;
	virtual void Update(float fFramTime) = 0;
	virtual void ProcessKeyEvent(EKeyId KeyId, int activatioMode, float value) = 0;

	virtual void OnExtended() {}
	virtual void OnReleased() {}

protected:
	IEntity *m_pEntity;

	bool m_bExtended = false;
	CPlayerComponent *m_pPlayerComponent = nullptr;

	//virtual void LinkPlayerComponent();

	enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};
	typedef uint16 TInputFlags;
	CFlags<TInputFlags> m_InputFlags;
	void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold);

	CHud* GetHud();

};
