#pragma once

#include "Types\Flags.h"

class CAI
{
public:

	CAI() {}
	virtual ~CAI() {}

#define AI_BIT(x) BIT64(x)
	using _FlagNumeric_ = int;
	using FlagType = CFlags<_FlagNumeric_>;
	enum EAIFlags
	{
		NONE = 0
	};

	virtual void Initialize() = 0;
	virtual void Update(float fDeltaTime) = 0;

	virtual FlagType GetDefaultFlagMask() const = 0;
	FlagType& GetFlags() { return m_Flags; }

protected:

	FlagType m_Flags;
};

