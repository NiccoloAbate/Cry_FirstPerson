#pragma once

#include <DefaultComponents\Cameras\CameraComponent.h>
#include <CryEntitySystem\IEntity.h>

class CCameraController
{
public:

	IEntity *m_pCameraEntity; // The game camera

	void OverheadCameraView(); //sets camera to overhead, top down view
	void MiniMapCameraView(); //sets camera to overhead, low fov view

	Cry::DefaultComponents::CCameraComponent* GetCameraComponent();
	CCamera& GetCamera();
};
