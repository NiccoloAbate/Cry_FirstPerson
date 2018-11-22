#pragma once

#define _IS_POINTER(Type) std::is_pointer<Type>::value

#define PROPERTY(Type, Name)					\
	protected:										\
		Type m_##Name;								\
	public:											\
		Type Get##Name(){return m_##Name;}			\
		void Set##Name(Type Val){m_##Name = Val;}	\

#define PROPERTY_PREFIX(Type, Name, Prefix)			\
	protected:												\
		Type m_##Prefix##Name;								\
	public:													\
		Type Get##Name(){return m_##Prefix##Name;}			\
		void Set##Name(Type Val){m_##Prefix##Name = Val;}	\

#define PROPERTY_DEFAULTVAL(Type, Name, DefaultVal)		\
	protected:													\
		Type m_##Name = Type(DefaultVal);						\
	public:														\
		Type Get##Name(){return m_##Name;}						\
		void Set##Name(Type Val){m_##Name = Val;}				\

