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


class CAI;
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

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_COLLISION) | BIT64(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED) |
		BIT64(ENTITY_EVENT_PRE_SERIALIZE) | BIT64(ENTITY_EVENT_POST_SERIALIZE) | BIT64(ENTITY_EVENT_INIT); }
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

	// Player
	CCharacter_PlayerExtension* GetOrCreatePlayerExtension() { return (m_pPlayerExtension) ? (m_pPlayerExtension) : (GivePlayerExtension()); }
	//~Player

	// Game
	CStatsComponent* GetStatsComponent() { return m_pStatsComponent; }
	CGameplayEntityComponent* GetGameplayEntityComponent() { return m_pGameplayEntityComponent; }
	//~Game

	// AI
	CAI* GetAI() const { return m_pAI; }
	// Add an auto destroy old functionality if needed
	void SetAI(CAI* const pAI) { m_pAI = pAI; }
	//~AI

	// Animation & Physics
	Cry::DefaultComponents::CAdvancedAnimationComponent* GetAnimationComponent() const { return m_pAnimationComponent; }
	Cry::DefaultComponents::CCharacterControllerComponent* GetCharacterControllerComponent() const { return m_pCharacterController; }

	void SetIdleFragmentID(FragmentID ID) { m_idleFragmentId = ID; }
	void SetWalkFragmentID(FragmentID ID) { m_walkFragmentId = ID; }
	void SetRotateTagID(FragmentID ID) { m_rotateTagId = ID; }
	void SetCameraJointID(int ID) { m_cameraJointId = ID; }

	void Ragdollize();
	//~Animation & Physics

protected:

	bool NeedGameSerialize() override { return true; }
	void GameSerialize(TSerialize ser) override;

	void Update(float fDeltaTime);
	// Temporary fix to the reseting model issue
	void FirstUpdate();

	// Player
	CCharacter_PlayerExtension *m_pPlayerExtension = nullptr;
	CCharacter_PlayerExtension* GivePlayerExtension();
	//~Player

	// Character
	CSpeechBubble *m_pSpeechBubble = nullptr;
	CGameplayEntityComponent *m_pGameplayEntityComponent = nullptr;
	//~Character

	// Game
	CStatsComponent *m_pStatsComponent = nullptr;
	MinMaxVar<float> m_Health{ 0, 100, 100 };
	//~Game

	// AI
	CAI *m_pAI = nullptr;
	//~AI

	//void UpdateMovementRequest(float frameTime);
	//void UpdateLookDirectionRequest(float frameTime);
	//void UpdateAnimation(float frameTime);

	// Animation & Physics
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	//~Animation & Physics

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

private:

	bool m_bLoadedFromSerialization = false;
	bool m_bFirstUpdateDone = false;
};
