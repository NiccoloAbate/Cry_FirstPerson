#include "StdAfx.h"
#include "HiveAgent_Deprecated.h"

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>

#include <CryMath\Random.h>

#include "Hive.h"
#include "Pheromone.h"
#include "GamePlugin.h"
#include "Components\Game\GameplayEntity.h"

#include "Utils\DebugUtils.h"
#include "Utils\MathUtils.h"
#include "Utils\Utils.h"

static void RegisterHiveAgentComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CHiveAgentComponent_Deprecated));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHiveAgentComponent)


#include "AI\Hive\HiveAgent\HiveAgent_AI.h"
void CHiveAgentComponent_Deprecated::Initialize()
{
	m_pEntity->LoadGeometry(0, "Objects/Default/primitive_sphere.cgf");

	// Pheromones
	m_PheromoneSilhouetteColor = cry_random_uint32();
	//~Pheromones

	// Pathfinding
	//gEnv->pAISystem->GetNavigationSystem()-
	//~Pathfinding

	m_pGameplayEntityComponent = m_pEntity->GetOrCreateComponent<CGameplayEntityComponent>();

	m_pAI = new CHiveAgent_AI(m_pEntity);
	m_pAI->Initialize();
}

#include <CryRenderer\RenderObject.h>
void CHiveAgentComponent_Deprecated::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_UPDATE:
	{

		if (gEnv->IsEditor())
			return;

		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		UpdateAI();
		UpdateMovement(pCtx->fFrameTime);
	}
	break;
	case ENTITY_EVENT_LEVEL_LOADED:
	{
		m_pParentHive = CGamePlugin::gGamePlugin->m_pHive;
		Utils::EntitySilhouette(m_pEntity, uint32(0x0000ffff));
	}
	break;
	}
}

void CHiveAgentComponent_Deprecated::Explore()
{
	m_CurrentBehavior = DefaultBehaviors::EXPLORE;
	m_bBehaviorToEnd = false;
	ForceAIImportantUpdate();
}

void CHiveAgentComponent_Deprecated::GoHome()
{
	//m_pParentHive->GetEntity()->GetWorldPos();
	m_CurrentBehavior = DefaultBehaviors::GOHOME;
	m_bBehaviorToEnd = false;
	ForceAIImportantUpdate();
}

void CHiveAgentComponent_Deprecated::UpdateAI()
{

	//Temporary time filter
	float Time = gEnv->pTimer->GetFrameStartTime().GetMilliSeconds();
	m_pAI->Update(Time);
	if (!ClampRange(m_LastAIUpdate, Time, AIUPDATE_TIMEFILTER))
		return;
	m_LastAIUpdate = Time;

	SensePheromones();

	//If movement is fallowing a path, check if next destination is reached
	if (m_pCurrentPheromoneDestination) {
		if (GetDistanceFromPheromone(m_pCurrentPheromoneDestination) <= 1.2)
		{
			if (m_bBehaviorToEnd)
				EndBehavior();
			ForceAIImportantUpdate();
		}
	}

	//Important Update Filter
	if (m_AIUpdatesSinceLastImportant == AIUPDATE_IMPORTANT_FILTER)
		m_AIUpdatesSinceLastImportant = 0;
	else
	{
		m_AIUpdatesSinceLastImportant++;
		return;
	}

	//Important AIUpdate Start

	switch (m_CurrentBehavior)
	{
	case NONE:
		return;
		break;
	case EXPLORE:
		Explore_Behavior();
		break;
	case GOHOME:
		GoHome_Behavior();
		break;
	default:
		break;
	}

	auto INFO = GETDEBUGINFO(m_SensedPheromones);

	bool STOP = true;

}

void CHiveAgentComponent_Deprecated::ForceAIImportantUpdate()
{
	m_AIUpdatesSinceLastImportant = AIUPDATE_IMPORTANT_FILTER;
}

void CHiveAgentComponent_Deprecated::UpdateMovement(float FrameTime)
{
	if (!m_bIsMoving)
		return;

	Vec3 Dir = m_pEntity->GetForwardDir().scale(m_MoveSpeed * FrameTime);
	const Vec3 Pos = m_pEntity->GetWorldPos();

	m_pEntity->SetPos(Pos + Dir);

	/*
	//Vec3 velocity = ZERO;
	//velocity.y += m_MoveSpeed * FrameTime * 1; // 100 = 10 x mass

	if (IPhysicalEntity* pPhysicalEntity = m_pEntity->GetPhysicalEntity())
	{
		pe_action_move moveAction;

		// Apply movement request directly to velocity
		moveAction.iJump = 2;
		moveAction.dir = { 0, 1000000, 0 };

		// Dispatch the movement request
		pPhysicalEntity->Action(&moveAction);
	}
	*/

}

void CHiveAgentComponent_Deprecated::SensePheromones()
{
	std::vector<CPheromoneComponent*> &Pheromones = CGamePlugin::gGamePlugin->m_pPheromoneManager->m_pPheromones;
	std::vector<CPheromoneComponent*> SensedPheromones;

	const Vec2 AgentPos = m_pEntity->GetPos();

	for (int i = 0; i < Pheromones.size(); i++) {

		CPheromoneComponent *pPheromoneEnt = Pheromones[i];
		Vec2 PheromonePos = pPheromoneEnt->GetEntity()->GetPos();
		if (ClampRange(AgentPos, PheromonePos, SENSERANGE_CLAMP))
			continue;

		SensedPheromones.push_back(pPheromoneEnt);
	}

	m_SensedPheromones = SensedPheromones;
}

CPheromoneComponent * CHiveAgentComponent_Deprecated::DropPheromone(int PheromoneType)
{
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.vPosition = m_pEntity->GetPos();

	CPheromoneComponent *pPheromone = nullptr;

	// Spawn the entity
	if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
	{
		pPheromone = pEntity->CreateComponentClass<CPheromoneComponent>();
		if (pPheromone)
		{
			pPheromone->SetTypeId(PheromoneType);
			pPheromone->m_pParentAgent = this;
			Utils::EntitySilhouette(pPheromone->GetEntity(), m_PheromoneSilhouetteColor);
		}
	}
	return pPheromone;
}

CPheromoneComponent * CHiveAgentComponent_Deprecated::GetStrongestPheromone(int PheromoneType)
{
	CPheromoneComponent *pClosest = nullptr;
	float HighestRelativeStrength;

	for (int x = 0; x < m_SensedPheromones.size(); x++) {

		if (m_SensedPheromones[x]->GetTypeId() != PheromoneType)
			continue;

		if (!pClosest) {
			pClosest = m_SensedPheromones[x];
			const float Dist = GetDistanceFromPheromone(m_SensedPheromones[x]);
			HighestRelativeStrength = m_SensedPheromones[x]->m_Strength * (((float)SENSERANGE - (Dist * .5f)) / (float)SENSERANGE); // .5f is how much the distance affects the strength of the pheromone
			continue;
		}

		if (m_SensedPheromones[x]->m_Strength <= HighestRelativeStrength)
			continue;
		const float Dist = GetDistanceFromPheromone(m_SensedPheromones[x]);
		const float RelativeStrength = m_SensedPheromones[x]->m_Strength * (((float)SENSERANGE - (Dist * .5f)) / (float)SENSERANGE); // .5f is how much the distance affects the strength of the pheromone
		if (RelativeStrength > HighestRelativeStrength)
		{
			HighestRelativeStrength = RelativeStrength;
			pClosest = m_SensedPheromones[x];
		}
	}

	return pClosest;
}

std::vector<CPheromoneComponent*> CHiveAgentComponent_Deprecated::GetPheromones(int PheromoneType)
{
	std::vector<CPheromoneComponent*> P;
	for (int x = 0; x < m_SensedPheromones.size(); x++)
	{
		if (m_SensedPheromones[x]->m_TypeId == PheromoneType)
			P.push_back(m_SensedPheromones[x]);
	}
	return P;
}

void CHiveAgentComponent_Deprecated::RemovePheromoneFromSensed(CPheromoneComponent * pPheromone)
{
	if (!pPheromone)
		return;
	auto it = std::find(m_SensedPheromones.begin(), m_SensedPheromones.end(), pPheromone);
	if (it != m_SensedPheromones.end())
		m_SensedPheromones.erase(it);
}

void CHiveAgentComponent_Deprecated::RemovePheromonesFromSensed(std::vector<CPheromoneComponent*> pPheromones)
{
	for (int x = 0; x < pPheromones.size(); x++) {
		auto it = std::find(m_SensedPheromones.begin(), m_SensedPheromones.end(), pPheromones[x]);
		if (it != m_SensedPheromones.end())
			m_SensedPheromones.erase(it);
	}
}

float CHiveAgentComponent_Deprecated::GetDistanceFromPheromone(CPheromoneComponent * pPheromone)
{
	Vec3 vDist = m_pEntity->GetPos() - pPheromone->GetEntity()->GetPos();
	return vDist.GetLength();
}

void CHiveAgentComponent_Deprecated::EndBehavior()
{
	switch (m_CurrentBehavior)
	{
	case NONE:
		break;
	case EXPLORE:
		Explore_End();
		break;
	case GOHOME:
		GoHome_End();
		break;
	default:
		break;
	}
}

bool UsePreviousPath_Function(float Strength)
{
	float fRand = cry_random<float>(0, 1);
	return (fRand >= Strength);
}
float ExploreCurve_Function(float x)
{
	return pow(x, 5); // Later implement a 9, 7, 5, 3 performance checker?
}
void CHiveAgentComponent_Deprecated::Explore_Behavior()
{
	if (m_bBehaviorToEnd) {
		DropPheromone(CPheromoneComponent::DefaultTypes::HOME_PATH);
		return;
	}

	if (auto pFood = GetStrongestPheromone(CPheromoneComponent::DefaultTypes::FOOD))
	{
		Utils::EntityFaceAt(m_pEntity, pFood->GetEntity());
		SetNewPheromoneDestination(pFood);
		m_bBehaviorToEnd = true;
		DropPheromone(CPheromoneComponent::DefaultTypes::HOME_PATH);
		return;
	}

	m_bIsMoving = true;
	Ang3 Ypr;
	Ypr.x = (float)(cry_random<int>(-100, 100)) / (float)100; // # between -1 and 1; 3 decimal digits
	Ypr.x = ExploreCurve_Function(Ypr.x);
	Ypr.x *= (float)0.8333 * (float)Pi; //Multiply by range clamp .8333; Multiply by pi to make it in radians

	Ypr.y = 0; Ypr.z = 0;

	Quat Rotation = m_pEntity->GetRotation() * Quat(CCamera::CreateOrientationYPR(Ypr));
	m_pEntity->SetRotation(Rotation);

	DropPheromone(CPheromoneComponent::DefaultTypes::HOME_PATH);

}

void CHiveAgentComponent_Deprecated::Explore_End()
{
	GoHome();
}

void CHiveAgentComponent_Deprecated::GoHome_Behavior()
{
	if (m_bBehaviorToEnd) {
		DropPheromone(CPheromoneComponent::DefaultTypes::FOOD_PATH);
		return;
	}

	RemovePheromonesFromSensed(m_pCurrentPathMemory);

	CPheromoneComponent *pHomeTagPheromone = GetStrongestPheromone(CPheromoneComponent::DefaultTypes::HOME);
	if (pHomeTagPheromone == m_pParentHive->m_pPheromoneHomeTag)
	{
		Utils::EntityFaceAt(m_pEntity, pHomeTagPheromone->GetEntity());
		SetNewPheromoneDestination(pHomeTagPheromone);
		m_bBehaviorToEnd = true;
		return;
	}

	CPheromoneComponent *pClosestHomePath = GetStrongestPheromone(CPheromoneComponent::DefaultTypes::HOME_PATH);
	Utils::EntityFaceAt(m_pEntity, pClosestHomePath->GetEntity());

	SetNewPheromoneDestination(pClosestHomePath);
	DropPheromone(CPheromoneComponent::DefaultTypes::FOOD_PATH);
}

void CHiveAgentComponent_Deprecated::GoHome_End()
{

	m_CurrentBehavior = DefaultBehaviors::NONE;
	m_bIsMoving = false;

}

void CHiveAgentComponent_Deprecated::SetNewPheromoneDestination(CPheromoneComponent * pPheromone)
{
	m_pCurrentPathMemory.insert(m_pCurrentPathMemory.begin(), pPheromone);
	if (m_pCurrentPathMemory.size() >= m_CurrentPathMemorySize)
		m_pCurrentPathMemory.erase(m_pCurrentPathMemory.begin() + (m_pCurrentPathMemory.size() - 1));
	m_pCurrentPheromoneDestination = pPheromone;
}

