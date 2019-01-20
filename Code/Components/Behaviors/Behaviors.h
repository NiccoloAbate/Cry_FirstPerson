#pragma once

#include "Utils\ExposeVariables.h"

class Behavior
{
public:

	virtual float GetBahaviorValue() { return 0; }

};

class CHiveAgentComponent_Deprecated;
class HiveAgentBehaviors : public Behavior
{
public:



};

