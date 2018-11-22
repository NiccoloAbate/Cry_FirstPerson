#include "StdAfx.h"
#include "Hive.h"

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
#include <CryEntitySystem\IEntity.h>

#include "Components\Hive\Pheromone.h"
#include "GamePlugin.h"


static void RegisterHiveComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CHiveComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHiveComponent)

void CHiveComponent::Initialize()
{

	GetEntity()->LoadGeometry(0, "Objects/Default/primitive_sphere.cgf");
	GetEntity()->SetScale({ 10, 10, 10 });

	m_pPheromoneHomeTag = m_pEntity->GetOrCreateComponent<CPheromoneComponent>();
	m_pPheromoneHomeTag->SetTypeId(CPheromoneComponent::DefaultTypes::HOME);
	m_pPheromoneHomeTag->m_bDoesDecay = false;

	CGamePlugin::gGamePlugin->m_pHive = this;

	/*//Spawn Pheromones
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();

	spawnParams.vPosition = m_pEntity->GetPos();

	m_pPheromoneHomeTag = nullptr;
	if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
	{
		m_pPheromoneHomeTag = pEntity->CreateComponentClass<CPheromoneComponent>();
		if (m_pPheromoneHomeTag)
		{
			m_pPheromoneHomeTag->SetTypeId(CPheromoneComponent::DefaultTypes::HOME);
			m_pPheromoneHomeTag->m_bDoesDecay = false;
		}
	}
	*/

}

void CHiveComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_UPDATE:
	{

	}
	break;
	}
}