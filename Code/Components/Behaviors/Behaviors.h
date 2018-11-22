#pragma once

#include "Utils\ExposeVariables.h"

class Behavior
{
public:

	virtual float GetBahaviorValue() { return 0; }

};

class CHiveAgentComponent;
class HiveAgentBehaviors : public Behavior
{
public:



};

