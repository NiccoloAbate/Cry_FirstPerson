#include "StdAfx.h"
#include "PlayerPhase.h"

#include "CombatGameflow.h"
#include "CombatGameflowPhases.h"

#include "Components\Characters\Character.h"
#include "Components\Game\Stats_Deprecated.h"


#include "Combat/Abilities.h"
#include "Combat/AbilityFactory.h"

#include "GamePlugin.h"
#include "Components/Player/Player.h"
#include "Components/Player/PlayerExtension.h"

#include "Utils/Utils.h"
#include "Utils/VectorUtils.h"

#include <CryInput\IHardwareMouse.h>
#include <DefaultComponents\AI\PathfindingComponent.h>
#include "GamePlugin.h"
#include "Components/Game/GameplayEntity.h"
#include "Components\Player\Player.h"
#include "Components\Player\PlayerExtension.h"
#include "Components\AI\Pathfinder.h"
#include "Components/Game/GameplayEntity.h"
#include "Types/Stats.h"

void CCombatGameflow_PlayerPhase::Initialize()
{
	GAMEFLOWPHASE_STANDARD_INIT();

	m_pTestAbility = CAbilityFactory::MakeAbility(CAbilityFactory::eAbilities::SLASH);
	m_pTestAbility->GetTargeting().SetUserEntity(CGamePlugin::gGamePlugin->GetPlayerComponent()->GetCameraEntity());

}

void CCombatGameflow_PlayerPhase::OnPhaseStart()
{
	CCharacterComponent *pCharacter = ((CCombatGameflow*)GetParentGameflow())->GetCharacter();
	CGamePlugin::gGamePlugin->GetPlayerExtensionEntity()->GetComponent<CGameplayEntityComponent>()->GetStamina().ResetToMax();
}

void CCombatGameflow_PlayerPhase::InitializeKeyBinds()
{
	Cry::DefaultComponents::CInputComponent	*pInputComponent = GetInputComponent();

	pInputComponent->RegisterAction(PHASENAME, "MovementTest", [this](int activationMode, float value) { MovementTest(); });
	pInputComponent->BindAction(PHASENAME, "MovementTest", eAID_KeyboardMouse, EKeyId::eKI_Mouse1, true, false, false);

	pInputComponent->RegisterAction(PHASENAME, "EndTurn", [this](int activationMode, float value) { if(((CCombatGameflow*)GetParentGameflow())->IsPlayerOutOfMoves()) EndPhase(); }); // Ends turn if player is out of moves
	pInputComponent->BindAction(PHASENAME, "EndTurn", eAID_KeyboardMouse, EKeyId::eKI_Enter, false, true, false);
	
	pInputComponent->RegisterAction(PHASENAME, "TestAbility", [this](int activationMode, float value) { TestAbility(); });
	pInputComponent->BindAction(PHASENAME, "TestAbility", eAID_KeyboardMouse, EKeyId::eKI_Mouse1, false, true, false);

	pInputComponent->RegisterAction(PHASENAME, "ShiftTarget_Next", [this](int activationMode, float value) { ShiftTarget(true); });
	pInputComponent->BindAction(PHASENAME, "ShiftTarget_Next", eAID_KeyboardMouse, EKeyId::eKI_Tab, true, false, false);
	pInputComponent->RegisterAction(PHASENAME, "ShiftTarget_Prev", [this](int activationMode, float value) { ShiftTarget(false); });
	pInputComponent->BindAction(PHASENAME, "ShiftTarget_Prev", eAID_KeyboardMouse, EKeyId::eKI_LShift, true, false, false);
}

void CCombatGameflow_PlayerPhase::Update(float fDeltaTime)
{
	ViewTrace();
}

void CCombatGameflow_PlayerPhase::EndPhase()
{
	GetParentGameflow()->StartNextPhase(Gameflow::CombatGameflow::Phases::PLAYER_END);
}

void Silhouette_On(IEntity* pEntity)
{
	Utils::EntitySilhouette(pEntity, 0xff000011);
}
void Silhouette_Off(IEntity* pEntity)
{
	Utils::EntitySilhouette(pEntity, 0x00000000);
}

Vec3 GetMouseRay()
{
	float MouseX;
	float MouseY;
	gEnv->pHardwareMouse->GetHardwareMousePosition(&MouseX, &MouseY);
	MouseY = gEnv->pRenderer->GetHeight() - MouseY;

	return gEnv->pRenderer->UnprojectFromScreen((int)MouseX, (int)MouseY);
}


void CCombatGameflow_PlayerPhase::MovementTest()
{
	Vec3 Ray = GetMouseRay() * 100;

	ray_hit Hit;
	IPhysicalEntity* pSkipEnt = CGamePlugin::gGamePlugin->GetPlayerExtensionEntity()->GetPhysicalEntity(); // Player Entity
	int HitCount = gEnv->pPhysicalWorld->RayWorldIntersection(gEnv->pRenderer->GetCamera().GetPosition(), Ray,
		ent_all, NULL, &Hit, 1, pSkipEnt);

	if (HitCount)
	{
		//IEntity* ent = gEnv->pEntitySystem->GetEntityFromPhysics(Hit.pCollider);

		if (IEntity *pHiveAgent01 = gEnv->pEntitySystem->FindEntityByName("HiveAgent 01"))
		{
			if (CPathfinderComponent *pPathfinderComponent = pHiveAgent01->GetComponent<CPathfinderComponent>())
			{
				if (pPathfinderComponent->IsProcessingRequest())
					pPathfinderComponent->CancelCurrentRequest();
				pPathfinderComponent->RequestMoveTo(Hit.pt);
				//pHiveAgent01->SetPos(Hit.pt);
			}
		}
	}
}

void CCombatGameflow_PlayerPhase::ViewTrace()
{
	IEntity* pPlayerEnt = CGamePlugin::gGamePlugin->GetPlayerExtensionEntity();
	Vec3 Ray = gEnv->pRenderer->GetCamera().GetViewdir() * (m_pTestAbility->GetTargeting().Get_BasicTargeting()->Range);

	ray_hit Hit;
	IPhysicalEntity* pSkipEnt = pPlayerEnt->GetPhysicalEntity(); // Player Entity
	int HitCount = gEnv->pPhysicalWorld->RayWorldIntersection(gEnv->pRenderer->GetCamera().GetPosition(), Ray,
		ent_all, NULL, &Hit, 1, pSkipEnt);

	if (!HitCount)
	{
		if (m_pTargetEntity)
		{
			Silhouette_Off(m_pTargetEntity);
			m_pTargetEntity = nullptr;
		}
		return;
	}

	if (IEntity * pHitEnt = gEnv->pEntitySystem->GetEntityFromPhysics(Hit.pCollider))
	{
		if (pHitEnt == m_pTargetEntity)
			return;

		// If there is an old target disable it's silhouette
		if (m_pTargetEntity)
			Silhouette_Off(m_pTargetEntity);

		m_pTargetEntity = pHitEnt;
		Silhouette_On(m_pTargetEntity);
	}

}

void CCombatGameflow_PlayerPhase::TestAbility()
{
	if (!m_pTargetEntity)
		return;

	m_pTestAbility->GetTargeting().SetTargetEntity(m_pTargetEntity);
	m_pTestAbility->Activate();
}

void CCombatGameflow_PlayerPhase::ShiftTarget(bool bNext)
{
	CCombatGameflow* pCombatGameflow = (CCombatGameflow*)GetParentGameflow();
	std::vector<IEntity*>& Enemies = pCombatGameflow->GetEnemies();

	if (Enemies.size() == 0)
		return;

	int StartIndex;
	
	if (!m_pTargetEntity)
		StartIndex = 0;
	else
	{
		StartIndex = VectorUtils::GetIndex(Enemies, m_pTargetEntity);
		if (StartIndex == -1)
			StartIndex = 0;
	}

	CPlayerComponent* pPlayerComponent = CGamePlugin::gGamePlugin->GetPlayerComponent();
	IEntity* pCameraEnt = pPlayerComponent->GetCameraEntity();
	int i = StartIndex;
	do
	{
		if (++i == Enemies.size())
			i = 0;
		
		if (pCameraEnt->GetWorldPos().GetDistance(Enemies[i]->GetWorldPos()) <= m_pTestAbility->GetTargeting().Get_BasicTargeting()->Range)
		{
			if(m_pTargetEntity)
				Silhouette_Off(m_pTargetEntity);
			m_pTargetEntity = Enemies[i];
			Silhouette_On(m_pTargetEntity);
			pPlayerComponent->FaceAt(m_pTargetEntity);
			break;
		}

	} while (i != StartIndex);


}
