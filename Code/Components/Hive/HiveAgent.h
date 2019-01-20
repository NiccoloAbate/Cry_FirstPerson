#pragma once


#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>
#include <vector>

#include <DefaultComponents\Effects\ParticleComponent.h>

#include "Utils\Macros\Properties.h"

class CCharacterComponent;
class CHiveAgent_AI;

////////////////////////////////////////////////////////
// Pheromone entity
////////////////////////////////////////////////////////
class CHiveAgentComponent final : public IEntityComponent
{
	friend class CHiveAgentComponent;
public:
	CHiveAgentComponent() = default;
	virtual ~CHiveAgentComponent() {}

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CHiveAgentComponent>& desc)
	{
		desc.SetGUID("{B1622B5D-BA9B-4941-8F11-256B3F82CFB8}"_cry_guid);
		desc.SetEditorCategory("Hive");
		desc.SetLabel("HiveAgentJJJJJJJJ");
		desc.SetDescription("Hive Angent");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	CCharacterComponent* GetCharacterComponent() const { return m_pCharacterComponent; }
	CHiveAgent_AI* GetHiveAgentAI() const { return m_pHiveAgentAI; }

protected:

	CCharacterComponent *m_pCharacterComponent;
	CHiveAgent_AI *m_pHiveAgentAI;
	
};

