#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include "Utils\Macros\Properties.h"
//#include "Utils\ExposeVariables.h"
#include "Components\Behaviors\Behaviors.h"

class CHiveComponent;
class CPheromoneComponent;
class CGameplayEntityComponent;

class CHiveAgent_AI;

#define Pi 3.1415926535897

////////////////////////////////////////////////////////
// Hive agent component
////////////////////////////////////////////////////////
class CHiveAgentComponent final : public IEntityComponent
{
public:
	CHiveAgentComponent() = default;
	virtual ~CHiveAgentComponent() {}

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CHiveAgentComponent>& desc)
	{
		desc.SetGUID("{AE97ABA8-17AA-42AF-8449-ED1DB4C2C39B}"_cry_guid);
		desc.SetEditorCategory("Hive");
		desc.SetLabel("HiveAgent");
		desc.SetDescription("The Agents of the hive");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_LEVEL_LOADED); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent


	CHiveAgent_AI *m_pAI;



	// Hive
	PROPERTY_PREFIX(CHiveComponent*, ParentHive, p) //the hive the agent belongs to
	void Explore();
	void GoHome();
	// ~Hive

	// AI
#define AIUPDATE_TIMEFILTER 100 // 1/10 second [in milliseconds]
#define AIUPDATE_IMPORTANT_FILTER 30 // 3 seconds [30 x AIUPDATE_TIMEFILTER]
	void UpdateAI();
	void ForceAIImportantUpdate();
	// ~AI

	// Movement
	PROPERTY_DEFAULTVAL(float, MoveSpeed, 3) // in Meters/Second
	bool m_bIsMoving = false;
	void UpdateMovement(float FrameTime);
	CPheromoneComponent *m_pCurrentPheromoneDestination = nullptr;
	bool m_bBehaviorToEnd = false;
	std::vector<CPheromoneComponent*> m_pCurrentPathMemory; // 0 = m_pCurrentPheromoneDestination; 1 is last, 2 is last before that, etc.
	static constexpr int m_CurrentPathMemorySize = 10;
	// ~Movement

	// Pheromones
#define SENSERANGE_CLAMP 20 // First value used to clamp sensed pheromones; used in ClampRange
#define SENSERANGE 28.2842712475 // sqrt(SENSERANGE_CLAMP^2 + SENSERANGE_CLAMP^2)
	void SensePheromones();
	CPheromoneComponent *DropPheromone(int PheromoneType = -1);
	CPheromoneComponent *GetStrongestPheromone(int PheromoneType);
	std::vector<CPheromoneComponent*> GetPheromones(int PheromoneType); //Gets all pheromones from sensed of a certain type
	// ~Pheromones

protected:
	// Character
	CGameplayEntityComponent *m_pGameplayEntityComponent = nullptr;
	//~Character

	// Pheromones
	std::vector<CPheromoneComponent*> m_SensedPheromones;
	void RemovePheromoneFromSensed(CPheromoneComponent *pPheromone);
	void RemovePheromonesFromSensed(std::vector<CPheromoneComponent*> pPheromones);
	float GetDistanceFromPheromone(CPheromoneComponent *pPheromone);
	// ~Pheromones

	//Behaviors
	enum DefaultBehaviors
	{
		NONE = -1,
		EXPLORE = 0,
		GOHOME,

		LASTDEFAULT,
	};
	int m_CurrentBehavior = EXPLORE;
	void EndBehavior();
	void Explore_Behavior();
	void Explore_End();
	void GoHome_Behavior();
	void GoHome_End();
	//~Behaviors

private:
	// AI
	float m_LastAIUpdate;
	int m_AIUpdatesSinceLastImportant = AIUPDATE_IMPORTANT_FILTER;
	// ~AI

	//Movement
	void SetNewPheromoneDestination(CPheromoneComponent *pPheromone);
	//~Movement

	// Pheromones
	uint32 m_PheromoneSilhouetteColor;
	//~Pheromones
};


