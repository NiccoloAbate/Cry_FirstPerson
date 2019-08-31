#pragma once

#include "CryEntitySystem/IEntity.h"
#include "Components\Timeline\TimelineEntity.h"

#include "Combat/CombatSystem.h"

////////////////////////////////////////////////////////
// Physicalized bullet shot from weaponry, expires on collision with another object
////////////////////////////////////////////////////////
using namespace Combat;

class CPlayerComponent;

class CEncounterSpawnPoint final : public IEntityComponent
{
public:
	virtual ~CEncounterSpawnPoint() {}

	// IEntityComponent
	virtual void Initialize() override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CEncounterSpawnPoint>& desc)
	{
		desc.SetGUID("{69B10A5C-3DA0-4699-A13C-236ECDB2C286}"_cry_guid);
		desc.SetEditorCategory("Combat");
		desc.SetLabel("EncounterSpawnPoint");
		desc.SetDescription("Spawnpoint for combat encounters");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

		desc.AddMember(&CEncounterSpawnPoint::m_iUnitType, 'ityp', "iUnitType", "Unit Type", "Type of Unit to Spawn [ID]", eUnitTypes::Minion);
	}

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_EDITOR_PROPERTY_CHANGED); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	void SetiUnitType(eUnitTypes UnitType) { m_iUnitType = UnitType; }
	unsigned int GetiUnitType() { return m_iUnitType; }

	void MoveTo(IEntity* pUnit);
	void MovePlayerTo();

	IEntity* SpawnAt();

	IEntity* GetConnectedEntity() { return m_pConnectedEntity; }

protected:

	IEntity* m_pConnectedEntity = nullptr;

	string m_GeomFilePath = "";
	void Reset();
	
	unsigned int m_iUnitType = Minion;

};
