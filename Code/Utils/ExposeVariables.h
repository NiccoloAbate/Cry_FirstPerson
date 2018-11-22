#pragma once

class PExposeVariables
{
public:
	enum eEV_IDs{};
	typedef uintptr_t pointer;
	virtual pointer GetVariable(int Id) = 0;
};

#define EXPOSE_VARIABLE_BYID(Name, Id)			\
	case ##Id:									\
		return pointer(&##Name);				\
	break;										\

#define EXPOSE_VARIABLE(Name)					\
	case eEV_##Name:							\
		return pointer(&##Name);				\
	break;										\

