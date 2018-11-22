#pragma once

#include "ExtensionFunctions.h"

class CSerializationEntension
{
public:
	
	typedef std::function<void(Serialization::IArchive&)> SerializationFunction;
	typedef SerializationExtensions Extensions;

	void Serialize(Serialization::IArchive &ar);

	void AddSerializationExtension(SerializationFunction Function);

private:
	std::vector<SerializationFunction> m_SerializationFunctions;
};
