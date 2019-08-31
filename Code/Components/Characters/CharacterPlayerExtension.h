#pragma once

#include "Components\Player\PlayerExtension.h"

//#include <CryEntitySystem\IEntityComponent.h>
//#include <DefaultComponents\Physics\CharacterControllerComponent.h>
//#include <DefaultComponents\Geometry\AdvancedAnimationComponent.h>

#include "Types\MovingAverage.h"


class CCharacterComponent;

class CCharacter_PlayerExtension : public CPlayerExtension
{
public:

	CCharacter_PlayerExtension(IEntity *pEntity, CCharacterComponent *pCharacterComponent) : CPlayerExtension(pEntity), m_pCharacterComponent(pCharacterComponent) {}
	virtual ~CCharacter_PlayerExtension() {}


	enum class EInputFlag
		: TInputFlags
	{
		MoveLeft = 1 << 0,
		MoveRight = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack = 1 << 3,
		Sprint = 1 << 4,
		LSHIFTDOWN = 1 << 4,
		LCTRLDOWN = 1 << 5,
		ALTDOWN = 1 << 6,
	};

	virtual void Initialize() override;
	virtual void Update(float fFrameTime) override;
	virtual void ProcessKeyEvent(EKeyId KeyId, int activationMode, float value) override;

	virtual void FaceAt(IEntity* pEntity, float fTime = 0) override;
	virtual void SetViewDir(Vec3 ViewDir, float fTime = 0) override;

protected:
	CCharacterComponent *m_pCharacterComponent;

	void UpdateMovementRequest(float frameTime);
	void UpdateLookDirectionRequest(float frameTime);
	void UpdateAnimation(float frameTime);
	void UpdateCamera(float frameTime);

	void Jump();
	
};

