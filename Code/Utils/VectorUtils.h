#pragma once

#include <vector>

struct VectorUtils
{

	template<typename T>
	static bool HasElement(std::vector<T>& Vec, T& Element)
	{
		return std::find(Vec.begin(), Vec.end(), Element) != Vec.end();
	}

	template<typename T>
	static int GetIndex(std::vector<T>& Vec, T& Element)
	{
		auto It = std::find(Vec.begin(), Vec.end(), Element);
		if (It == Vec.end())
			return -1;
		return (int) std::distance(Vec.begin(), It);
	}

};

