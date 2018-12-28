#include <StdAfx.h>
#include "CameraController.h"

#include "Utils\ConsoleCommands.h"
#include "GamePlugin.h"

void CCameraController::OverheadCameraView()
{
	const int MidPoint = gEnv->p3DEngine->GetTerrainSize() / 2;
	const int Height = 100;
	m_pCameraEntity->DetachThis();
	m_pCameraEntity->SetPos(Vec3((float)MidPoint, (float)MidPoint, (float)Height));
	const Quat Down = m_pCameraEntity->GetRotation().CreateRotationVDir(Vec3(0, 0, -1));
	m_pCameraEntity->SetRotation(Quat(Down));
}

void CCameraController::MiniMapCameraView()
{
	const int MidPoint = gEnv->p3DEngine->GetTerrainSize() / 2;
	const int Height = 400;
	m_pCameraEntity->DetachThis();
	m_pCameraEntity->SetPos(Vec3((float)MidPoint, (float)MidPoint, (float)Height));
	const Quat Down = m_pCameraEntity->GetRotation().CreateRotationVDir(Vec3(0, 0, -1));
	m_pCameraEntity->SetRotation(Quat(Down));

	CryTransform::CAngle FOV = CryTransform::CAngle::FromDegrees(0);
	GetCameraComponent()->SetFieldOfView(FOV);
}

Cry::DefaultComponents::CCameraComponent * CCameraController::GetCameraComponent()
{
	return m_pCameraEntity->GetComponent<Cry::DefaultComponents::CCameraComponent>();
}

CCamera & CCameraController::GetCamera()
{
	return GetCameraComponent()->GetCamera();
}

// Console Commands
void CC_OverheadCamera(CC_Args pArgs)
{
	CGamePlugin::gGamePlugin->m_pCameraController->OverheadCameraView();
}
ADDCONSOLECOMMAND("Cam_Overhead", CC_OverheadCamera)

void CC_MiniMapCamera(CC_Args pArgs)
{
	CGamePlugin::gGamePlugin->m_pCameraController->MiniMapCameraView();
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("Cam_MiniMap", 0), CC_MiniMapCamera)

void CC_Cam_FOV(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1);
	CC_GETARG(float, fAngle, 1);
	CryTransform::CAngle FOV = CryTransform::CAngle::FromDegrees(fAngle);
	CGamePlugin::gGamePlugin->m_pCameraController->GetCameraComponent()->SetFieldOfView(FOV);
}
ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(CC_Info("Cam_FOV", 0), CC_Cam_FOV)
//~Console Commands