#pragma once


template<typename T>
struct _NULLREF_
{
	static T& GetRef()
	{
		static T NullRef;
		return NullRef;
	}
};
#define NULLREF(Type) _NULLREF_<Type>::GetRef()
