#include "StdAfx.h"
#include "SerializationEnxtension.h"



void CSerializationEntension::Serialize(Serialization::IArchive & ar)
{
	for (int i = 0; i < m_SerializationFunctions.size(); i++)
	{
		m_SerializationFunctions[i](ar);
	}

	//SerializationExtension(ar);
}

void CSerializationEntension::AddSerializationExtension(SerializationFunction Function)
{
	m_SerializationFunctions.push_back(Function);
}
