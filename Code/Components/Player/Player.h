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
	enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};

	typedef uint16 TInputFlags;
	enum class EInputFlag
		: TInputFlags
	{
		MoveLeft = 1 << 0,
		MoveRight = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack = 1 << 3,
		Sprint = 1 << 4,
		SHIFTDOWN = 1 << 4,
		CONTROLDOWN = 1 << 5,
		ALTDOWN = 1 << 6,
		TimeUp = 1 << 7,
		TimeDown = 1 << 8,
	};

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

	//void Revive();
	//void SetActiveChar();
	//void SetInactiveChar();
	//PROPERTY_DEFAULTVAL(bool, Active, true)

	//void SetCamera(IEntity *pCamera);

	//CStatsComponent* GetStatsComponent() { return m_pStatsComponent; }

	// World Interaction
	//IEntity *m_pLookEntity = nullptr;
	//~World Interaction

protected:
	//void UpdateMovementRequest(float frameTime);
	//void UpdateKeyBindRequests(float frameTime);
	void UpdateLookDirectionRequest(float frameTime);
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

	CStatsComponent *m_pStatsComponent = nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

	TInputFlags m_inputFlags;
	Vec2 m_mouseDeltaRotation;
	MovingAverage<Vec2, 10> m_mouseDeltaSmoothingFilter;
#define MOUSE_DELTA_TRESHOLD 0.0001f

	const float m_rotationSpeed = 0.002f;

	int m_cameraJointId = -1;

	FragmentID m_activeFragmentId;

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;

private:
	//CHud* GetHud();
};

