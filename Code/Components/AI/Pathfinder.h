#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntity.h>

#include <functional>
#include "Types\Flags.h"

#include "Utils\Debug\DebugExtension.h"


struct SMovementParams
{
	float MoveSpeed = 3.2;
	float EndRadius = .1;
};

class CPathfinderComponent final : public IEntityComponent
{
public:
	CPathfinderComponent() = default;
	virtual ~CPathfinderComponent() {}
	
	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CPathfinderComponent>& desc)
	{
		desc.SetGUID("{E99F57C4-6245-4A28-8727-E4839C31406B}"_cry_guid);
		desc.SetEditorCategory("AI");
		desc.SetLabel("PathfinderComponent");
		desc.SetDescription("My custom pathfinding component");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	}

	using MovementCallbackType = std::function<void(Vec3)>;
	using DestinationCallbackType = std::function<void()>;

	enum ePathfindingMethods
	{
		STRAIGHTLINE = 0,
	};

	using _FlagNumeric_ = short;
	using FlagType = CFlags<_FlagNumeric_>;
	enum ePathfindingFlags
	{
		NONE = 0,
		GROUNDED = BIT64(1),
	};

	DEBUGEXTENSION()

	// IEntityComponent
	virtual void Initialize() override;

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_UPDATE); }
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~IEntityComponent

	void SetMovementCallback(MovementCallbackType Callback) { m_MovementCallback = Callback; }

	ePathfindingMethods GetPathfindingMethod() const { return m_PathfindingMethod; }
	void SetPathfindingMethod(ePathfindingMethods Method) { m_PathfindingMethod = Method; }
	FlagType& GetPathfindingFlags() { return m_PathindingFlags; }

	bool IsProcessingRequest() const { return m_bHasRequest; }
	void RequestMoveTo(Vec3 Destination) { m_bHasRequest = true; m_CurrentRequestedDestination = Destination; RequestMoveTo_Debug(Destination); }
	void CancelCurrentRequest() { m_bHasRequest = false; }

	void SetMovementParams(SMovementParams &Params) { m_MovementParams = Params; }
	SMovementParams& GetMovementParams() { return m_MovementParams; }

protected:
	bool m_bHasRequest = false;
	Vec3 m_CurrentRequestedDestination;

	SMovementParams m_MovementParams;

	// Highest level update function
	void Update();

	// Methods
	void Update_StraightLine(Vec3 &RecommendedVelocity);
	//~Methods

	MovementCallbackType m_MovementCallback = NULL;
	DestinationCallbackType m_DestinationCallback = NULL;

	ePathfindingMethods m_PathfindingMethod;
	FlagType m_PathindingFlags;

	// Debug
	void RequestMoveTo_Debug(Vec3 Destination);
	//~Debug

};
