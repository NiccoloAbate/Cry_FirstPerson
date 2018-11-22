#include <StdAfx.h>
#include "CameraController.h"

#include "Utils\ConsoleCommands.h"
#include "GamePlugin.h"

void CCameraController::OverheadCameraView()
{
	const int MidPoint = gEnv->p3DEngine->GetTerrainSize() / 2;
	const int Height = 100;
	m_pCamera->DetachThis();
	m_pCamera->SetPos(Vec3((float)MidPoint, (float)MidPoint, (float)Height));
	const Quat Down = m_pCamera->GetRotation().CreateRotationVDir(Vec3(0, 0, -1));
	m_pCamera->SetRotation(Quat(Down));
}

// Console Commands
void CC_OverheadCamera(CC_Args pArgs)
{
	CGamePlugin::gGamePlugin->m_pCameraController->OverheadCameraView();
}
ADDCONSOLECOMMAND("Cam_Overhead", CC_OverheadCamera)
//~Console Commands