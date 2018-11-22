#pragma once

#include <CrySystem\ISystem.h>
#include <vector>
#include <CryEntitySystem\IEntity.h>

#define JOINXY(x, y) x ## y

struct CC_Info
{
	CC_Info(string name = "") { Name = name; }
	CC_Info(string name, int numparams) { Name = name; NumParams = numparams; }
	string Name;
	int NumParams = -1;
	string Format()
	{
		string Return;
		if (NumParams == -1)
			Return = Name;
		else
		{
			Return = Name + ": " + ToString(NumParams) + "P";
		}
		return Return;
	}
};

class CC_Manager
{
public:
	static std::vector<CC_Info> CommandInfo;
	static void AddCommand(CC_Info Info);
	static CC_Info& GetCC_Info(string CommandName);
	static CC_Info& GetInfoBuffer();
private:
	static CC_Info NULL_CC_INFO;
};

typedef IConsoleCmdArgs* CC_Args;

//Adds a command with no special info
#define ADDCONSOLECOMMAND(strName, FuncName)													\
static void RegisterConsoleCommand_##FuncName(Schematyc::IEnvRegistrar& registrar)				\
{																								\
	ConsoleRegistrationHelper::AddCommand(strName, FuncName);									\
	CC_Manager::AddCommand(CC_Info(strName));													\
}																								\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterConsoleCommand_##FuncName)							\

//Adds a command that isn't logged to help
#define ADDCONSOLECOMMAND_HIDDEN(strName, FuncName)												\
static void RegisterConsoleCommand_##FuncName(Schematyc::IEnvRegistrar& registrar)				\
{																								\
	ConsoleRegistrationHelper::AddCommand(strName, FuncName);									\
}																								\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterConsoleCommand_##FuncName)							\

//Adds a command with info from a global info instance made right before the macro call
#define ADDCONSOLECOMMAND_WITHINFOCLASS(Info, FuncName)											\
static void RegisterConsoleCommand_##FuncName(Schematyc::IEnvRegistrar& registrar)				\
{																								\
	ConsoleRegistrationHelper::AddCommand(Info.Name, FuncName);									\
	CC_Manager::AddCommand(Info);																\
}																								\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterConsoleCommand_##FuncName)							\

//Adds a command with info from a global info instance made right before the macro call
#define ADDCONSOLECOMMAND_WITHINFOCONSTRUCTOR(InfoConstructor, FuncName)						\
static void RegisterConsoleCommand_##FuncName(Schematyc::IEnvRegistrar& registrar)				\
{																								\
	CC_Info Info = InfoConstructor;																\
	ConsoleRegistrationHelper::AddCommand(Info.Name, FuncName);									\
	CC_Manager::AddCommand(Info);																\
}																								\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterConsoleCommand_##FuncName)							\

//Must go after ADDCONSOLECOMMAND_WITHINFOBUFFER
#define CC_SETINFOBUFFER(strName, numParams)													\
static void JOINXY(CC_SetBufferInfo_, __COUNTER__)(Schematyc::IEnvRegistrar& registrar)			\
{																								\
	CC_Manager::GetInfoBuffer() = CC_Info(strName, numParams);									\
}																								\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&JOINXY(CC_SetBufferInfo_, __COUNTER__))						\

//Adds a command with info from the info buffer; CC_SETINFOBUFFER must go after the macro call
#define ADDCONSOLECOMMAND_WITHINFOBUFFER(FuncName)												\
static void RegisterConsoleCommand_##FuncName(Schematyc::IEnvRegistrar& registrar)				\
{																								\
	CC_Info Info = CC_Manager::GetInfoBuffer();													\
	ConsoleRegistrationHelper::AddCommand(Info.Name, FuncName);									\
	CC_Manager::AddCommand(Info);																\
}																								\
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterConsoleCommand_##FuncName)							\

#define CC_ARGSCOUNTCHECK(x)																	\
if (pArgs->GetArgCount() <= x)																	\
{																								\
	gEnv->pLog->LogError("Not Enough Parameters; " + ToString(x) + " Expected");				\
	return;																						\
}																								\

inline bool FromString(string &val, const char *s) { val = string(s); return (val != ""); }

#define CC_GETARG(Type, Name, ArgNum)					\
Type Name;												\
FromString(Name, pArgs->GetArg(ArgNum));				\

//Example
/*void Test(CC_Args pArgs)
{
	bool Worked = true;
}
ADDCONSOLECOMMAND("test", Test)*/