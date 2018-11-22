#pragma once


#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>



class CSerializationEntension;

////////////////////////////////////////////////////////
// Pheromone entity
////////////////////////////////////////////////////////
class CTimelineEntityComponent final : public IEntityComponent
{
public:
	CTimelineEntityComponent() = default;
	virtual ~CTimelineEntityComponent() {}

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CTimelineEntityComponent>& desc)
	{
		desc.SetGUID("{0B190094-8000-4052-863D-CFE74F6D31A5}"_cry_guid);
		desc.SetEditorCategory("Game");
		desc.SetLabel("Timeline");
		desc.SetDescription("Timeline Component");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(0); }
	virtual void ProcessEvent(SEntityEvent& event) override {}
	// ~IEntityComponent

	void AddSerializationExtension(std::function<void(Serialization::IArchive&)> Function);

	void AddEntityTransformFunction();
	void AddEntityPhysicsFunction();

protected:

	void Register();

	CSerializationEntension *m_pSerializationExtension;

};

