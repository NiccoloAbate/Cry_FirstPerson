#include "StdAfx.h"
#include "Stats.h"

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


static void RegisterStatsComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CStatsComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterStatsComponent)




void CStatsComponent::Initialize()
{
	//InitializeUI();
}

void CStatsComponent::ProcessEvent(SEntityEvent & event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
	{
		/*
		IEntityComponent *pComponent = (IEntityComponent*)event.nParam[0];
		const Schematyc::CClassMemberDesc *pMemberDesc = pComponent->GetClassDesc().FindMemberById((uint32)event.nParam[1]);
		const string Name = pMemberDesc->GetName();
		if (Name.find("PARAM_") != 0)
			return;
		const Key Id = pMemberDesc->GetId();
		if (!HasParam(Id))
			return;
		const ptrdiff_t Offset = pMemberDesc->GetOffset();
		float *pParam = (float*)((UINT_PTR)pComponent + Offset);
		float Param = *pParam;
		float OldParam = GetParam<float>(Id);
		SetParam<float>(Id, Param);
		float NewParam = GetParam<float>(Id);
		*/
	}
	break;
	default:
		break;
	}
}

void CStatsComponent::InitializeUI()
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
