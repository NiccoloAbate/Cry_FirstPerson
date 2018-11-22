#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include "Types\MinMaxVar.h"

class CEntityHudElement_StatBar;
class CTimelineEntityComponent;
class CStatsComponent;

////////////////////////////////////////////////////////
// Basic Gameplay Entity component
////////////////////////////////////////////////////////
class CGameplayEntityComponent final : public IEntityComponent
{
public:
	CGameplayEntityComponent() = default;
	virtual ~CGameplayEntityComponent() {}

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CGameplayEntityComponent>& desc)
	{
		desc.SetGUID("{AC491E04-6E29-411D-8FCD-9097BAE10E43}"_cry_guid);
		desc.SetEditorCategory("Game");
		desc.SetLabel("GameplayEntity");
		desc.SetDescription("A basic gameplay entity");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
		
		//desc.AddMember(&CGameplayEntityComponent::m_Health, 'hlth', "health", "Health", "Entity Health", Stat<float>());
		ReflectParams(desc);
	}
	static void ReflectParams(Schematyc::CTypeDesc<CGameplayEntityComponent>& desc);

	// Stats
	template<typename T>
	using Stat = MinMaxVar<T>;

	Stat<float> GetHealth() { return m_Health; }
	// Sets the whole Stat. Updates flash
	void SetHealthStat(Stat<float> Health);
	void SetHealthMin(float Min);
	void SetHealthMax(float Max);
	// Sets the current Stat value
	void SetHealth(float Health);
	//~Stats

	CTimelineEntityComponent* GetTimelineComponent() { return m_pTimelineComponent; }
	CStatsComponent* GetStats() { return m_pStatsComponent; }

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

protected:
	// Stats
	Stat<float> m_Health{ 0, 100, 100 };
	CEntityHudElement_StatBar *m_pStatBar = nullptr;
	
	void InitializeUI();
	//~Stats

	CTimelineEntityComponent *m_pTimelineComponent = nullptr;
	CStatsComponent *m_pStatsComponent = nullptr;

public:
};
