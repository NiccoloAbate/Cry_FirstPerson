#include "StdAfx.h"
#include "GameplayEntity.h"

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

#include "GamePlugin.h"
#include "Game\UIController.h"
#include "UI\EntityHud.h"
#include "Components\Timeline\TimelineEntity.h"
#include "Components\Game\Stats.h"


static void RegisterGameplayEntityComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CGameplayEntityComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterGameplayEntityComponent)



void CGameplayEntityComponent::ReflectParams(Schematyc::CTypeDesc<CGameplayEntityComponent>& desc)
{
	REFLECTPARAM(&CGameplayEntityComponent::m_Health, Health, Stat<float>());
}

void CGameplayEntityComponent::SetHealthStat(Stat<float> Health)
{
	m_Health = Health;
	if(m_pStatBar)
		m_pStatBar->SetStat(Health);
}

void CGameplayEntityComponent::SetHealthMin(float Min)
{
	m_Health.Min = Min;
	if (m_pStatBar)
		m_pStatBar->SetStatMin(Min);
}

void CGameplayEntityComponent::SetHealthMax(float Max)
{
	m_Health.Max = Max;
	if (m_pStatBar)
		m_pStatBar->SetStatMax(Max);
}

void CGameplayEntityComponent::SetHealth(float Health)
{
	m_Health = Health;
	if (m_pStatBar)
		m_pStatBar->SetStat(Health);
}

void CGameplayEntityComponent::Initialize()
{
	m_pTimelineComponent = m_pEntity->GetOrCreateComponent<CTimelineEntityComponent>();
	m_pStatsComponent = m_pEntity->GetOrCreateComponent<CStatsComponent>();

	m_pStatsComponent->SetHealth({ 0, 100, 87 });

	const Schematyc::CClassMemberDescArray &Members = GetClassDesc().GetMembers();
	for (int i = 0; i < Members.size(); i++)
	{
		const Schematyc::CClassMemberDesc &Member = Members[i];
		const string Name = Member.GetName();
		if (Name.find("PARAM_") != 0) continue;
		const CStatsComponent::Key Id = Member.GetId();
		//if (!m_pStatsComponent->HasParam(Id)) continue;
		const ptrdiff_t Offset = Member.GetOffset();
		UINT_PTR Ptr = ((UINT_PTR)this + Offset);
		m_pStatsComponent->BindParamPTR(Id, Ptr);
	}

	InitializeUI();
}

void CGameplayEntityComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
		UPDATEREFLECTEDPARAMS();
	break;
	default:
		break;
	}
}

void CGameplayEntityComponent::InitializeUI()
{
	std::function<Vec3()> PosFunc;
	bool bHasEntityHudAttachment = false;
	if (ICharacterInstance *pCharacter = m_pEntity->GetCharacter(0))
	{
		if (IAttachment *pEntityHudAttachment = pCharacter->GetIAttachmentManager()->GetInterfaceByName("EntityHud_Attachment"))
		{
			PosFunc = [this]() {return m_pEntity->GetCharacter(0)->GetIAttachmentManager()->GetInterfaceByName("EntityHud_Attachment")->GetAttWorldAbsolute().t; };
			bHasEntityHudAttachment = true;
		}
	}
	if (!bHasEntityHudAttachment)
	{
		AABB Box;
		m_pEntity->GetWorldBounds(Box);
		float Height = Box.GetSize().z;
		PosFunc = [this]() 
		{
			AABB Box;
			m_pEntity->GetWorldBounds(Box);
			float Height = Box.GetSize().y * .8f;
			return m_pEntity->GetWorldPos() + Vec3(0, 0, Height);
		};
	}
	
	CEntityHudManager *pEntityHudManager = CGamePlugin::gGamePlugin->m_pUIController->m_pEntityHudManager;
	m_pStatBar = pEntityHudManager->NewStatBar(string(m_pEntity->GetName()) + "_HealthBar", "Health", 0, 0, "Health", m_Health);
	m_pStatBar->m_bPosFunc = true;
	m_pStatBar->m_PosFunc = PosFunc;
}
