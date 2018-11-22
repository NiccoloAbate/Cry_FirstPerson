#pragma once

#include "Types\MovingAverage.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Input/InputComponent.h>

#include "Utils\Macros\Properties.h"

#define ACTIVECHECK if(!GetActive()) return;
#define ACTIVECHECK_RETURNVAL(Return) if(!GetActive()) return Return;
#define INPUTFLAGCHECK(Flag) if (!(m_inputFlags & (TInputFlags)EInputFlag::##Flag)) return;
#define INPUTFLAGCHECK_NOT(Flag) if(m_inputFlags & (TInputFlags)EInputFlag::##Flag) return;


class CStatsComponent;
class CHud;

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CCryFirstPersonPlayerComponent final : public IEntityComponent
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
	CCryFirstPersonPlayerComponent() = default;
	virtual ~CCryFirstPersonPlayerComponent() {}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CCryFirstPersonPlayerComponent>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
	}

	void Revive();
	void SetActiveChar();
	void SetInactiveChar();
	PROPERTY_DEFAULTVAL(bool, Active, true)

	void SetCamera(IEntity *pCamera);

	CStatsComponent* GetStatsComponent() { return m_pStatsComponent; }

	// World Interaction
	IEntity *m_pLookEntity = nullptr;
	//~World Interaction

protected:
	void UpdateMovementRequest(float frameTime);
	void UpdateKeyBindRequests(float frameTime);
	void UpdateLookDirectionRequest(float frameTime);
	void UpdateAnimation(float frameTime);
	void UpdateCamera(float frameTime);
	void Update(float frameTime);

	void SpawnAtSpawnPoint();

	void Jump();

	void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold);

	// World Interaction
	void UpdateLookEntity(float frameTime = -1);
	void RemoveLookEntityHighlight();
	//~World Interaction

protected:
	//Pointer to different entity's camera
	IEntity *m_pCamera = nullptr;
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

	CStatsComponent *m_pStatsComponent = nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

	TInputFlags m_inputFlags;
	Vec2 m_mouseDeltaRotation;
	MovingAverage<Vec2, 10> m_mouseDeltaSmoothingFilter;

	const float m_rotationSpeed = 0.002f;

	int m_cameraJointId = -1;

	FragmentID m_activeFragmentId;

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;

private:
	CHud* GetHud();
};
