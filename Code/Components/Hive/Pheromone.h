#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>
#include <vector>

#include <DefaultComponents\Effects\ParticleComponent.h>

#include "Utils\Macros\Properties.h"

class CHiveAgentComponent_Deprecated;

////////////////////////////////////////////////////////
// Pheromone entity
////////////////////////////////////////////////////////
class CPheromoneComponent final : public IEntityComponent
{
	friend class CHiveAgentComponent_Deprecated;
public:
	CPheromoneComponent() = default;
	virtual ~CPheromoneComponent() {}

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CPheromoneComponent>& desc)
	{
		desc.SetGUID("{588ECE16-7A88-4509-BD46-DF211C1CA442}"_cry_guid);
		desc.SetEditorCategory("Hive");
		desc.SetLabel("Pheromone");
		desc.SetDescription("Pheromones placed by HiveAgents");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_LEVEL_LOADED); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	// Pheromone Traits
	enum DefaultTypes
	{
		NONE = -1,
		FOOD = 0,
		FOOD_PATH,
		DEATH,
		DEATH_PATH,
		HOME,
		HOME_PATH,

		LASTDEFAULT
	};
	
	PROPERTY_DEFAULTVAL(int, TypeId, DefaultTypes::NONE)
	bool IsResource() { return (m_TypeId == FOOD); } //Add more as more resources are added

	CHiveAgentComponent_Deprecated *m_pParentAgent = nullptr;
	// ~Pheromone Traits

	//Decay
	bool m_bDoesDecay = true;
	//~Decay

protected:

	// Particle Effects
	Cry::DefaultComponents::CParticleComponent *m_pParticleComponent;
	//~Particle Effects

	// Decay
	float m_LastTime;

#define DECAYTIME 180000 /// 3 minutes [in milliseconds]
	float m_Strength = DECAYTIME;

	void Decay();
	// ~Decay

public:

	void Destroy();
};

class CPheromoneManager
{
public:
	std::vector<CPheromoneComponent*> m_pPheromones;
};
