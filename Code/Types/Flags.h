#pragma once

#include <type_traits>

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class CFlags
{
public:

	CFlags() = default;
	CFlags(T Flags) : m_Flags(Flags) {}

	void AddFlags(T Flags) { m_Flags |= Flags; }
	void RemoveFlags(T Flags) { m_Flags &= ~Flags; }
	bool HasFlags(T Flags) { return ((m_Flags | Flags) == m_Flags); } // For multiple bits
	bool HasFlag(T Flag) { return (m_Flags & Flag) != 0; } // For only 1 bit flag
	void ToggleFlags(T Flags) { m_Flags ^= Flags; }

protected:
	T m_Flags = 0;
};

