#include "StdAfx.h"
#include "Pheromone.h"

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

#include "GamePlugin.h"

static void RegisterPheromoneComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPheromoneComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPheromoneComponent)

void CPheromoneComponent::Initialize()
{

	const int nSlot = m_pEntity->GetSlotCount();
	m_pEntity->LoadGeometry(nSlot, "Objects/Hive/Pheromone/Pheromone.cgf");
	m_pEntity->SetViewDistRatio(255);

	m_LastTime = gEnv->pTimer->GetFrameStartTime().GetMilliSeconds();

	CGamePlugin::gGamePlugin->m_pPheromoneManager->m_pPheromones.push_back(this);

	//Temporary Particle Effects
	m_pParticleComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CParticleComponent>();
	m_pParticleComponent->SetEffectName("ParticleEffects/Hive/Pheromone/Pheromone.pfx");
	m_pParticleComponent->LoadEffect(true);
	m_pParticleComponent->Activate(false);
	//m_pParticleComponent->SetTransformMatrix(Matrix34(Vec3(.1, .1, .1), Quat(ZERO), Vec3(ZERO))); //Matrix34(?, Rot, Pos)

	if (string(m_pEntity->GetName()) == "Food 1")
	{
		SetTypeId(DefaultTypes::FOOD);
		m_pParticleComponent->SetEffectName("ParticleEffects/Hive/Test/Test.pfx");
		m_pParticleComponent->LoadEffect(true);
		m_pParticleComponent->Activate(true);
	}

	//IParticleEmitter* pEmitter = m_pParticleComponent->GetEntity()->GetParticleEmitter(m_pParticleComponent->GetEntitySlotId());
	//pEmitter->GetAttributes().

}

void CPheromoneComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_UPDATE:
	{
		Decay();
	}
	break;
	case ENTITY_EVENT_LEVEL_LOADED:
	{
		//m_pParticleComponent->Activate(false);
	}
	break;
	}
}

void CPheromoneComponent::Decay()
{
	
	if (!m_bDoesDecay)
		return;

	float Time = gEnv->pTimer->GetFrameStartTime().GetMilliSeconds();
	m_Strength -= (Time - m_LastTime);
	m_LastTime = Time;

	if (m_Strength <= 0)
		Destroy();

}

void CPheromoneComponent::Destroy()
{

	std::vector<CPheromoneComponent*> &Pheromones = CGamePlugin::gGamePlugin->m_pPheromoneManager->m_pPheromones;

	auto it = std::find(Pheromones.begin(), Pheromones.end(), this);
	if (it != Pheromones.end())
		Pheromones.erase(it);

	gEnv->pEntitySystem->RemoveEntity(GetEntityId());

}
