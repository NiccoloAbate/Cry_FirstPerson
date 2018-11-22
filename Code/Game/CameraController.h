#pragma once

#include <CryEntitySystem\IEntity.h>

class CCameraController
{
public:

	IEntity *m_pCamera; // The game camera

	void OverheadCameraView(); //sets camera to overhead, top down view

};
