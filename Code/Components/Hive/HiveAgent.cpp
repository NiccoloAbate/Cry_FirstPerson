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
#include "Components\Game\Stats_Deprecated.h"
#include "AI\Hive\HiveAgent\HiveAgent_AI.h"
#include "Components\AI\Pathfinder.h"

#include "Utils\Initialization\DeferredInitialization.h"

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
	
	if (gEnv->pGameFramework->IsInLevelLoad()) // just don't have to re-initialize the animation and physics information
		return;

	Cry::DefaultComponents::CAdvancedAnimationComponent* pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	Cry::DefaultComponents::CCharacterControllerComponent *pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();

	// Set the player geometry, this also triggers physics proxy creation
	pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/Ant_01/Ant_01.adb");
	pAnimationComponent->SetCharacterFile("Objects/Characters/Ant_01/Ant_01.cdf");

	pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/Ant_01/Ant_01ControllerDefinition.xml");
	pAnimationComponent->SetDefaultScopeContextName("Ant_01Character");
	// Queue the idle fragment to start playing immediately on next update
	pAnimationComponent->SetDefaultFragmentName("Idle");

	// Disable movement coming from the animation (root joint offset), we control this entirely via physics
	pAnimationComponent->SetAnimationDrivenMotion(false);

	// Load the character and Mannequin data from file
	pAnimationComponent->LoadFromDisk();

	// Acquire fragment and tag identifiers to avoid doing so each update
	m_pCharacterComponent->SetIdleFragmentID(pAnimationComponent->GetFragmentId("Idle"));
	m_pCharacterComponent->SetWalkFragmentID(pAnimationComponent->GetFragmentId("Walk"));
	m_pCharacterComponent->SetRotateTagID(pAnimationComponent->GetTagId("Rotate"));

	if (ICharacterInstance *pCharacter = pAnimationComponent->GetCharacter())
		m_pCharacterComponent->SetCameraJointID(pCharacter->GetIDefaultSkeleton().GetJointIDByName("Head"));

	pCharacterController->SetTransformMatrix(ZERO);
	
	//m_pCharacterComponent->GetAnimationComponent()->GetCharacter()->GetIAttachmentManager()->GetInterfaceByName("Light_Attachment");
	m_pProjectorLightComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CProjectorLightComponent>();
	m_pProjectorLightComponent->GetColorParameters().m_diffuseMultiplier = 100;
	m_pProjectorLightComponent->GetColorParameters().m_specularMultiplier = 100;
	m_pProjectorLightComponent->GetColorParameters().m_color = ColorF({ .8, 0, 1 });

	SEntityEvent Event;
	Event.event = ENTITY_EVENT_EDITOR_PROPERTY_CHANGED;
	m_pProjectorLightComponent->SendEvent(Event);
}

void CHiveAgentComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	default:
		break;
	case ENTITY_EVENT_LEVEL_LOADED:
		OnLevelLoaded();
	case ENTITY_EVENT_UPDATE:
		break;
	}
}

void CHiveAgentComponent::PostInit()
{
	m_pHiveAgentAI = new CHiveAgent_AI(m_pEntity);
	m_pHiveAgentAI->Initialize();
	m_pCharacterComponent->SetAI(m_pHiveAgentAI);
}

void CHiveAgentComponent::OnLevelLoaded()
{
	PostInit();
}
