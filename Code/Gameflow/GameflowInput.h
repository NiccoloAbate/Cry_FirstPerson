#pragma once

#include "CryInput\IInput.h"

#include <unordered_map>

struct SGameflowInputCallback
{
	using FunctionType = std::function<void(int activationMode, float value)>;

	SGameflowInputCallback() = default;
	SGameflowInputCallback(FunctionType function, bool onPress, bool onRelease, bool onHold) : Function(function)
	{
		if (onPress) States |= EInputState::eIS_Pressed;
		if (onRelease) States |= EInputState::eIS_Released;
		if (onHold) States |= EInputState::eIS_Down;
	}

	bool HasState(EInputState state)
	{
		return (States & (uint8)state) != 0;
	}

	FunctionType Function;
	uint8 States;
};

class CGameflowInputManager : public IInputEventListener
{
private:

	std::unordered_map<EKeyId, std::vector<SGameflowInputCallback>> m_Actions;

	//! Called every time input event is generated.
	//! \return true if the broadcasting of this event should be aborted and the rest of input listeners should not receive this event.
	virtual bool OnInputEvent(const SInputEvent& event) override;

};

