#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>


class CPheromoneComponent;

////////////////////////////////////////////////////////
// Hive component
////////////////////////////////////////////////////////
class CHiveComponent final : public IEntityComponent
{
public:
	CHiveComponent() = default;
	virtual ~CHiveComponent() {}

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CHiveComponent>& desc)
	{
		desc.SetGUID("{429242CB-8E83-4E2D-9B6C-11C1774B5D08}"_cry_guid);
		desc.SetEditorCategory("Hive");
		desc.SetLabel("Hive");
		desc.SetDescription("The hive");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	// Hive
	CPheromoneComponent *m_pPheromoneHomeTag;
	// ~Hive

	// Agents

protected:
public:
};