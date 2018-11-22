#include "StdAfx.h"
#include "TimelineEntity.h"

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

#include "Utils\Serialization\SerializationEnxtension.h"
#include "Utils\Serialization\ExtensionFunctions.h"
#include "GamePlugin.h"
#include "Timeline\TimelineManager.h"
#include "Timeline\FrameCaptureManager.h"

static void RegisterTimelineEntityComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CTimelineEntityComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterTimelineEntityComponent)

void CTimelineEntityComponent::Initialize()
{
	m_pSerializationExtension = new CSerializationEntension;
	AddEntityTransformFunction();
	Register();
}

void CTimelineEntityComponent::AddSerializationExtension(std::function<void(Serialization::IArchive&)> Function)
{
	m_pSerializationExtension->AddSerializationExtension(Function);
}

void CTimelineEntityComponent::AddEntityTransformFunction()
{
	m_pSerializationExtension->AddSerializationExtension(CSerializationEntension::Extensions::EntityTransform(m_pEntity));
}

void CTimelineEntityComponent::AddEntityPhysicsFunction()
{
	m_pSerializationExtension->AddSerializationExtension(CSerializationEntension::Extensions::EntityPhysics(m_pEntity));
}

void CTimelineEntityComponent::Register()
{
	CGamePlugin::gGamePlugin->m_pTimelineManager->m_pFrameCaptureManager->AddFrameCapturer(m_pSerializationExtension);
}
