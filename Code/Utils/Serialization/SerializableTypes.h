#pragma once

class Serializeable
{

};

template<typename T>
class SetGet_Serializable
{
public:

	typedef std::function<void(T)> SetFunction;
	typedef std::function<T()> GetFunction;
	
	SetGet_Serializable() = default;
	SetGet_Serializable(SetFunction SetFunc, GetFunction GetFunc)
	{
		m_SetFunction = SetFunc;
		m_GetFunction = GetFunc;
	}

	void Serialize(Serialization::IArchive &ar)
	{
		if (ar.isOutput())
			m_Buffer = m_GetFunction();
		ar(m_Buffer, "Buffer");
		if (ar.isInput())
			m_SetFunction(m_Buffer);
	}

protected:
	T m_Buffer;

	SetFunction m_SetFunction;
	GetFunction m_GetFunction;
};

/*
template<typename Obj, typename T>
class SetGet_Serializable
{
public:

	typedef void(Obj::*)(Vec3) SetFunction;
	typedef T(Obj::*)() GetFunction;

	SetGet_Serializable() = default;
	SetGet_Serializable(Obj *pObj, SetFunction SetFunc, GetFunction GetFunc)
	{
		m_pObj = pObj;
		m_SetFunction = SetFunc;
		m_GetFunction = GetFunc;
	}

	void Serialize(Serialization::IArchive &ar)
	{
		if (ar.isOutput())
			m_Buffer = m_GetFunction()
		ar(m_Buffer, "Buffer");
		if (ar.isInput())
			m_SetFunction(m_Buffer);
	}

protected:
	T m_Buffer;

	Obj *m_pObj;
	SetFunction m_SetFunction;
	GetFunction m_GetFunction;
};
*/