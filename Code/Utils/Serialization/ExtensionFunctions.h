#pragma once


struct SerializationExtensions
{
	typedef std::function<void(Serialization::IArchive&)> ExtensionFunction;

	static ExtensionFunction EntityTransform(IEntity *pEntity);
	static ExtensionFunction EntityPhysics(IEntity *pEntity);
};
