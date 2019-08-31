#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include "Types\BindableGeneric.h"
#include "Types\MinMaxVar.h"
#include "Utils\Macros\UtilityMacros.h"


template<typename T>
class ReflectedEditorParamWithCallback
{
public:
	ReflectedEditorParamWithCallback() = default;
	ReflectedEditorParamWithCallback(T Param) : m_Param(Param) {}

	T m_Param;
	std::function<void()> m_OnUpdate;

	ReflectedEditorParamWithCallback& operator=(const T Param)
	{
		m_Param = Param;
		m_OnUpdate();
		return *this;
	}

	static void ReflectType(Schematyc::CTypeDesc<ReflectedEditorParamWithCallback<T>>& desc)
	{
		desc.AddMember(&ReflectedEditorParamWithCallback<T>::m_Param, 'parm', desc.GetName(), desc.GetLabel(), desc.GetDescription(), desc.GetDevaultValue());
	}
};


struct StringHasher
{
	std::size_t operator()(const string& k) const
	{
		return std::hash<std::string>()(k.c_str());
	}
};


template<typename T>
using Stat = MinMaxVar<T>;

class CEntityHudElement_StatBar;


////////////////////////////////////////////////////////
// Basic Gameplay Entity component
////////////////////////////////////////////////////////
class CStatsComponent_Deprecated final : public IEntityComponent
{
public:
	CStatsComponent_Deprecated() = default;
	virtual ~CStatsComponent_Deprecated() {}

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CStatsComponent_Deprecated>& desc)
	{
		desc.SetGUID("{A48A4E3E-0084-4CF8-8335-0B77FE2A2259}"_cry_guid);
		desc.SetEditorCategory("Game");
		desc.SetLabel("Stats");
		desc.SetDescription("A component to store stats on an entity");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

	using ParamData = CBindableGeneric;
	using Key = int;
#define NULLKEY -1
	using Key2 = string;
	using Key2Hasher = StringHasher;
#define NULLKEY2 ""

#define DEFAULTPARAM(Name, Type)														\
bool Has##Name() { return HasParam(DefaultParams::##Name); }							\
void Set##Name(Type Name)	{ SetParam<Type>(DefaultParams::##Name, Name); }			\
Type& Get##Name() { return GetParam<Type>(DefaultParams::##Name); }						\
void Give##Name(Type Name) { m_Params[DefaultParams::##Name] = ParamData::New(Name); }

	enum DefaultParams
	{
		Health = 0,
		Stamina,

		LastDefault = Stamina,
	};
	DEFAULTPARAM(Health, Stat<float>)
	DEFAULTPARAM(Stamina, Stat<float>)

#define REFLECTPARAM(ClassMember, Name, DefaultValue) desc.AddMember(ClassMember, CStatsComponent_Deprecated::DefaultParams::##Name, "PARAM_"#Name, #Name, #Name, DefaultValue);
#define UPDATEREFLECTEDPARAMS()																										\
	{																																\
		IEntityComponent *pComponent = ((IEntityComponent*)event.nParam[0]) ? ((IEntityComponent*)event.nParam[0]) : (this);		\
		const Schematyc::CClassMemberDesc *pMemberDesc = pComponent->GetClassDesc().FindMemberById((uint32)event.nParam[1]);		\
		const string Name = pMemberDesc->GetName();																					\
		if (Name.find("PARAM_") != 0) return;																						\
		const CStatsComponent_Deprecated::Key Id = pMemberDesc->GetId();																		\
		if (!m_pStatsComponent->HasParam(Id)) return;																				\
		const ptrdiff_t Offset = pMemberDesc->GetOffset();																			\
		UINT_PTR Ptr = ((UINT_PTR)pComponent + Offset);																				\
		m_pStatsComponent->SetParamWithPTR(Id, Ptr);																				\
	}


	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	template <typename T>
	Key NewParam(T Val, Key2 Name = NULLKEY2)
	{
		Key KeyId = GetNextKey();
		m_Params[KeyId] = ParamData::New(Val);
		if (Name != NULLKEY2)
			m_Key2Map[Name] = KeyId;
		return KeyId;
	}
	template <typename T>
	Key NewParam(Key KeyId, T Val, Key2 Name = NULLKEY2)
	{
		m_Params[KeyId] = ParamData::New(Val);
		if (Name != NULLKEY2)
			m_Key2Map[Name] = KeyId;
		return KeyId;
	}
	template <typename T>
	void SetParam(Key Id, T Val)
	{
		if (!HasParam(Id))
			NewParam(Id, Val);
		else
			m_Params[Id]->Set(Val);
	}
	template <typename T>
	void SetParam(Key2 Name, T Val)
	{
		const Key Id = GetKey(Name);
		if (Id == NULLKEY) return;
		m_Params[Id]->Set(Val);
	}
	void SetParamWithPTR(Key Id, UINT_PTR Ptr) { if (!HasParam(Id)) return; m_Params[Id]->SetWithPTR(Ptr); }
	void SetParamWithPTR(Key2 Name, UINT_PTR Ptr) { const Key Id = GetKey(Name); if (!HasParam(Id)) return; m_Params[Id]->SetWithPTR(Ptr); }
	template <typename T>
	T& GetParam(Key Id) { if (!HasParam(Id)) return NULLREF(T); return m_Params[Id]->Get<T>(); }
	template <typename T>
	T& GetParam(Key2 Name) 
	{ 
		const Key Id = GetKey(Name); 
		if (Id == NULLKEY) return NULLREF(T); 
		return m_Params[Id]->Get<T>(); 
	}
	UINT_PTR GetParamPTR(Key Id) { if (!HasParam(Id)) return NULL; return m_Params[Id]->GetPTR(); }
	UINT_PTR GetParamPTR(Key2 Name) { const Key Id = GetKey(Name); if (!HasParam(Id)) return NULL; return m_Params[Id]->GetPTR(); }
	bool HasParam(Key Id) { return Map_HasElement(m_Params, Id); }
	bool HasParam(Key2 Name) { return Map_HasElement(m_Key2Map, Name); }

	void BindParamPTR(Key Id, UINT_PTR Ptr)
	{
		if (HasParam(Id))
		{
			m_Params[Id]->BindPTR(Ptr);
		}
		//m_BoundPtrs[Id].push_back(Ptr);
	}
	void BindParamPTR(Key2 Name, UINT_PTR Ptr)
	{
		const Key Id = GetKey(Name);
		if (HasParam(Id))
		{
			m_Params[Id]->BindPTR(Ptr);
		}
		//if (!HasParam(Id)) return;
		//m_BoundPtrs[Id].push_back(Ptr);
	}

protected:

	std::unordered_map<Key, ParamData*> m_Params;
	Key m_LastKey = LastDefault;
	Key GetNextKey() { return ++m_LastKey; }

	std::unordered_map<Key2, Key, Key2Hasher> m_Key2Map;
	Key GetKey(Key2 K2) { if (!Map_HasElement(m_Key2Map, K2)) return NULLKEY; return m_Key2Map[K2]; }

	//Stat<float> m_Health{ 0, 100, 100 };
	//CEntityHudElement_StatBar *m_pStatBar = nullptr;

	void InitializeUI();

private:

	template<typename Map, typename Key>
	bool Map_HasElement(Map & map, Key & key) { return (map.find(key) != map.end()); }
};

