#pragma once

#include <vector>

template<class T>
struct DEBUGINFO
{
	static const bool Specialized = false;
};

//VECTORS
template<class T>
struct DEBUGINFO<std::vector<T>>
{
	static const bool Specialized = true;
	int Size;
	T FirstElement; // Element0
	T Element1;
	T Element2;
	T LastElement;

	DEBUGINFO(std::vector<T> Vec) {
		Size = Vec.size();
		if (Size >= 1) {
			FirstElement = Vec[0];
			LastElement = Vec[Size - 1];
		}
		if (Size >= 2)
			Element1 = Vec[1];
		if (Size >= 3)
			Element2 = Vec[2];
	}
};

template<class T>
static DEBUGINFO<T> GETDEBUGINFO(T Var) { return DEBUGINFO<T>(Var); }
