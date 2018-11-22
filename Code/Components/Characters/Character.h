#pragma once

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

#include "Types\MinMaxVar.h"


class CSpeechBubble;
class CGameplayEntityComponent;
class CStatsComponent;

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CCharacterComponent final : public IEntityComponent
{

	template<typename T, size_t SAMPLES_COUNT>
	class MovingAverage
	{
		static_assert(SAMPLES_COUNT > 0, "SAMPLES_COUNT shall be larger than zero!");

	public:

		MovingAverage()
			: m_values()
			, m_cursor(SAMPLES_COUNT)
			, m_accumulator()
		{
		}

		MovingAverage& Push(const T& value)
		{
			if (m_cursor == SAMPLES_COUNT)
			{
				m_values.fill(value);
				m_cursor = 0;
				m_accumulator = std::accumulate(m_values.begin(), m_values.end(), T(0));
			}
			else
			{
				m_accumulator -= m_values[m_cursor];
				m_values[m_cursor] = value;
				m_accumulator += m_values[m_cursor];
				m_cursor = (m_cursor + 1) % SAMPLES_COUNT;
			}

			return *this;
		}

		T Get() const
		{
			return m_accumulator / T(SAMPLES_COUNT);
		}

		void Reset()
		{
			m_cursor = SAMPLES_COUNT;
		}

	private:

		std::array<T, SAMPLES_COUNT> m_values;
		size_t m_cursor;

		T m_accumulator;
	};

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

protected:

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

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

	Vec2 m_mouseDeltaRotation;
	MovingAverage<Vec2, 10> m_mouseDeltaSmoothingFilter;

	const float m_rotationSpeed = 0.002f;

	int m_cameraJointId = -1;

	FragmentID m_activeFragmentId;

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;
};
