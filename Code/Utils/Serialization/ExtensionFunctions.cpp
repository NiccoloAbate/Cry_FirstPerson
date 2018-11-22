#include "StdAfx.h"
#include "ExtensionFunctions.h"

#include <CryEntitySystem\IEntity.h>

SerializationExtensions::ExtensionFunction SerializationExtensions::EntityTransform(IEntity * pEntity)
{
	return [pEntity](Serialization::IArchive &ar)
	{
		Vec3 Pos;
		Quat Rot;

		if (ar.isOutput())
		{
			Pos = pEntity->GetPos();
			Rot = pEntity->GetRotation();
		}

		ar(Pos, "Pos");
		ar(Rot, "Rot");

		if (ar.isInput())
		{
			pEntity->SetPos(Pos);
			pEntity->SetRotation(Rot);
		}
	};
}

SerializationExtensions::ExtensionFunction SerializationExtensions::EntityPhysics(IEntity * pEntity)
{
	return [pEntity](Serialization::IArchive &ar)
	{
		
	};
}
