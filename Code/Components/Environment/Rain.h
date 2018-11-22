#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include <DefaultComponents\Effects\ParticleComponent.h>

#include <CryParticleSystem/IParticlesPfx2.h>

class CBindToCamera_PositionOnly : public pfx2::IParticleComponent
{
public:

};



////////////////////////////////////////////////////////
// Level Rain Component
////////////////////////////////////////////////////////
class CRainComponent final : public IEntityComponent
{
public:
	virtual ~CRainComponent() {}

	// Rain Parameters
	struct SRainParams
	{
		inline bool operator==(const SRainParams &rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		static void ReflectType(Schematyc::CTypeDesc<SRainParams>& desc)
		{
			desc.SetGUID("{59A0D68B-579C-488C-B96C-7E98890D0352}"_cry_guid);
			desc.SetLabel("Rain Parameters");

			desc.AddMember(&SRainParams::WindSpeed_X, 'spdx', "WindSpeed_X", "Artificial Wind Speed X", nullptr, 10);
			desc.AddMember(&SRainParams::WindSpeed_Y, 'spdy', "WindSpeed_Y", "Artificial Wind Speed Y", nullptr, 3);
			desc.AddMember(&SRainParams::AreaSize, 'area', "RainSpawnArea", "Rain Spawn Area in 10 * Meters0", nullptr, 5);
			//desc.AddMember(&SRainParams::bUseLevelWind, 'lvwi', "UseLevelWind", "Use Level Wind Or Custom Wind", nullptr, true);
			desc.AddMember(&SRainParams::Thickness, 'thic', "RainThickness", "The Width of the Rain Ribbons", nullptr, 1);
			desc.AddMember(&SRainParams::bActive, 'actv', "Active", "is Rain On Or Off", nullptr, true);
		}

		float WindSpeed_X = 10; // Min -10; Max 10
		float WindSpeed_Y = 3; // Min -10; Max 10
		float Thickness = 1; // Min 0; Max 5
		float AreaSize = 5; // Sets the box size for spawning rain = Radius * 10 Meters; Min 3; Max 7
		//bool bUseLevelWind = true;
		bool bActive = true;
	};
	void Reset();
	void SetWindSpeed_X(float X) { m_RainParams.WindSpeed_X = X; Reset(); }
	void SetWindSpeed_Y(float Y) { m_RainParams.WindSpeed_Y = Y; Reset(); }
	void SetWindSpeed(Vec2 XY) { m_RainParams.WindSpeed_X = XY.x; m_RainParams.WindSpeed_Y = XY.y; Reset(); }
	void SetThickness(float Thickness) { m_RainParams.Thickness = Thickness; Reset(); }
	//void UseLevelWind(bool bUseLevelWind) { m_RainParams.bUseLevelWind = bUseLevelWind; }
	void Activate(bool bActive) { m_RainParams.bActive = bActive; Reset(); }
	//~Rain Parameters

	// IEntityComponent
	virtual void Initialize() override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CRainComponent>& desc)
	{
		desc.SetGUID("{2103606E-F7B1-4C19-AFBE-8B2B9881EF35}"_cry_guid);

		desc.SetEditorCategory("Environment");
		desc.SetLabel("Rain");
		desc.SetDescription("Used For Level Rain");
		//desc.SetIcon("icons:schematyc/entity_particle_emitter_component.png");
		//desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach});

		desc.AddMember(&CRainComponent::m_RainParams, 'rain', "RainParams", "Rain Parameters", nullptr, CRainComponent::SRainParams());
	}

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED) | BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_LEVEL_LOADED); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

protected:
	Cry::DefaultComponents::CParticleComponent *m_pParticleComponent;
	SRainParams m_RainParams;
	IParticleEmitter* m_pEmitter;
};
