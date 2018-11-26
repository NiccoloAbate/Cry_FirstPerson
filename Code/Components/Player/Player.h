#pragma once


#include "Types\MovingAverage.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>

#include "Utils\Macros\Properties.h"


#define REGISTER_PLAYEREXTESION_KEYEVENT(KeyName, KeyId)																																														\
m_pInputComponent->RegisterAction("Player", "ExtensionKeyEvent_" KeyName, [this](int activationMode, float value) { if (m_pPlayerExtension) m_pPlayerExtension->ProcessKeyEvent(KeyId, activationMode, value); });								\
m_pInputComponent->BindAction("Player", "ExtensionKeyEvent_" KeyName, eAID_KeyboardMouse, KeyId, true, true, true);


class CPlayerExtension;
class CStatsComponent;
class CHud;

////////////////////////////////////////////////////////
// SINGLETON; Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayerComponent final : public IEntityComponent
{
public:
	CPlayerComponent() = default;
	virtual ~CPlayerComponent() {}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
	{
		desc.SetGUID("{C64460B8-9B99-41FB-B9D4-C9F28C206BD9}"_cry_guid);
	}

	Cry::DefaultComponents::CInputComponent* GetInputComponent() { return m_pInputComponent; }
	CPlayerExtension* GetPlayerExtension() { return m_pPlayerExtension; }

	void Release();
	void ExtendTo(CPlayerExtension *pPlayerExtension);

	IEntity* GetCameraEntity() { return m_pCamera; }

	CStatsComponent* GetStatsComponent();

	// World Interaction
	//IEntity *m_pLookEntity = nullptr;
	//~World Interaction

protected:
	//void UpdateMovementRequest(float frameTime);
	//void UpdateKeyBindRequests(float frameTime);
	//void UpdateLookDirectionRequest(float frameTime);
	//void UpdateAnimation(float frameTime);
	//void UpdateCamera(float frameTime);
	//void Update(float frameTime);

	//void SpawnAtSpawnPoint();

	//void Jump();

	//void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold);

	// World Interaction
	//void UpdateLookEntity(float frameTime = -1);
	//void RemoveLookEntityHighlight();
	//~World Interaction

protected:
	CPlayerExtension *m_pPlayerExtension = nullptr;

	//Pointer to different entity's camera
	IEntity *m_pCamera = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

private:
	//CHud* GetHud();
};

