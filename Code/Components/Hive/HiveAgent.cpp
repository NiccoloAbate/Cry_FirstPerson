#include "StdAfx.h"
#include "HiveAgent.h"

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

#include "Components\Characters\Character.h"
#include "Components\Game\Stats.h"
#include "AI\Hive\HiveAgent\HiveAgent_AI.h"

static void RegisterHiveAgentComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CHiveAgentComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHiveAgentComponent)

void CHiveAgentComponent::Initialize()
{
	m_pCharacterComponent = m_pEntity->GetOrCreateComponent<CCharacterComponent>();
	Cry::DefaultComponents::CAdvancedAnimationComponent* const pAnimationComponent = m_pCharacterComponent->GetAnimationComponent();

	// Set the player geometry, this also triggers physics proxy creation
	pAnimationComponent->SetMannequinAnimationDatabaseFile("Objects/Characters/Ant_01/Mannequin/ADB/Ant_01.adb");
	pAnimationComponent->SetCharacterFile("Objects/Characters/Ant_01/Ant_01.cdf");

	pAnimationComponent->SetControllerDefinitionFile("Objects/Characters/Ant_01/Mannequin/ADB/Ant_01ControllerDefinition.xml");
	pAnimationComponent->SetDefaultScopeContextName("Ant_01Character");
	// Queue the idle fragment to start playing immediately on next update
	pAnimationComponent->SetDefaultFragmentName("Idle");

	// Acquire fragment and tag identifiers to avoid doing so each update
	m_pCharacterComponent->SetIdleFragmentID(pAnimationComponent->GetFragmentId("Idle"));
	m_pCharacterComponent->SetWalkFragmentID(pAnimationComponent->GetFragmentId("Walk"));
	//m_rotateTagId = NULL; // m_pAnimationComponent->GetTagId("Rotate");

	// Disable movement coming from the animation (root joint offset), we control this entirely via physics
	pAnimationComponent->SetAnimationDrivenMotion(false);

	// Load the character and Mannequin data from file
	pAnimationComponent->LoadFromDisk();

	if (ICharacterInstance *pCharacter = pAnimationComponent->GetCharacter())
		m_pCharacterComponent->SetCameraJointID(pCharacter->GetIDefaultSkeleton().GetJointIDByName("Head"));

	m_pHiveAgentAI = new CHiveAgent_AI;
	m_pCharacterComponent->SetAI(m_pHiveAgentAI);
}

void CHiveAgentComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	default:
		break;
	case ENTITY_EVENT_UPDATE:
		break;
	}
}
