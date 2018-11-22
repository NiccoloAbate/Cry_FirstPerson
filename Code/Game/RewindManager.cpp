#include "StdAfx.h"
#include "RewindManager.h"

#include "Components\Rewind\Rewind.h"

#include "Utils\ConsoleCommands.h"


void CRewindManager::Test()
{

	DynArray<char> Buffer;
	Serialization::SaveBinaryBuffer(Buffer, *this);
	m_Name = "Tedly";
	Serialization::LoadBinaryBuffer(*this, Buffer.data(), Buffer.get_alloc_size());

}

void CRewindManager::Serialize(Serialization::IArchive & ar)
{
	ar(m_TimeScale, "timescale", "TimeScale");
	ar(m_Name, "name", "Name");
}

void CRewindManager::Rewind()
{
	IEntityIt *It = gEnv->pEntitySystem->GetEntityIterator();
	for (IEntity *pEntity = It->This(); !It->IsEnd(); pEntity = It->Next())
	{
		if (CRewindComponent *pRewind = pEntity->GetComponent<CRewindComponent>())
		{
			pRewind->StartRewinding();
		}
	}
}


