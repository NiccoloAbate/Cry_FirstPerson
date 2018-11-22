#pragma once

#include <CryEntitySystem\IEntity.h>
#include <DefaultComponents\Lights\EnvironmentProbeComponent.h>

class CRainComponent;

class CEnvironmentController
{
public:

	Cry::DefaultComponents::CEnvironmentProbeComponent *m_pProbe;
	CRainComponent *m_pRain;

	void Initialize();

	void SetDiffuseMultiplier(float fMultiplier);

};
