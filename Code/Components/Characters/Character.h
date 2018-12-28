#pragma once

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

#include "Types\MovingAverage.h"
#include "Types\MinMaxVar.h"


class CSpeechBubble;
class CGameplayEntityComponent;
class CStatsComponent;
class CCharacter_PlayerExtension;

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CCharacterComponent final : public IEntityComponent
{
	friend class CCharacter_PlayerExtension;

public:
	CCharacterComponent() = default;
	virtual ~CCharacterComponent() {}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_COLLISION) | BIT64(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CCharacterComponent>& desc)
	{
		desc.SetGUID("{62A23C32-9798-4181-AC50-0EC367D6D60C}"_cry_guid);
		desc.SetEditorCategory("Characters");
		desc.SetLabel("Character");
		desc.SetDescription("A basic character");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

		ReflectParams(desc);
	}
	static void ReflectParams(Schematyc::CTypeDesc<CCharacterComponent>& desc);
	// Character
	void Speak(string Text);
	//~Character

	// Physics
	void Ragdollize();
	//~Physics

	// Player
	CCharacter_PlayerExtension* GetOrCreatePlayerExtension() { return (m_pPlayerExtension) ? (m_pPlayerExtension) : (GivePlayerExtension()); }
	//~Player

	CStatsComponent* GetStatsComponent() { return m_pStatsComponent; }
	CGameplayEntityComponent* GetGameplayEntityComponent() { return m_pGameplayEntityComponent; }

protected:

	// Player
	CCharacter_PlayerExtension *m_pPlayerExtension = nullptr;
	CCharacter_PlayerExtension* GivePlayerExtension();
	//~Player

	// Character
	CSpeechBubble *m_pSpeechBubble = nullptr;
	CGameplayEntityComponent *m_pGameplayEntityComponent = nullptr;
	//~Character

	// Stats
	CStatsComponent *m_pStatsComponent = nullptr;
	MinMaxVar<float> m_Health{ 0, 100, 100 };
	//~Stats

	//void UpdateMovementRequest(float frameTime);
	//void UpdateLookDirectionRequest(float frameTime);
	//void UpdateAnimation(float frameTime);

	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;

	Vec2 m_mouseDeltaRotation;
	MovingAverage<Vec2, 10> m_mouseDeltaSmoothingFilter;
#define MOUSE_DELTA_TRESHOLD 0.0001f

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

	const float m_rotationSpeed = 0.002f;
	int m_cameraJointId = -1;

	FragmentID m_activeFragmentId;
};
