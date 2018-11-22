#include "StdAfx.h"
#include "ConsoleCommands.h"

#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvSignal.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>

std::vector<CC_Info> CC_Manager::CommandInfo = std::vector<CC_Info>();
CC_Info CC_Manager::NULL_CC_INFO = CC_Info();

void CC_ShowCommands(CC_Args pArgs)
{
	gEnv->pLog->Log(" ");
	const int NumCommands = CC_Manager::CommandInfo.size();
	for (int i = 0; i < NumCommands; i++)
	{
		gEnv->pLog->Log(CC_Manager::CommandInfo[i].Format());
	}
	gEnv->pLog->Log(" ");
}
ADDCONSOLECOMMAND("ShowCommands", CC_ShowCommands)
void CC_Help(CC_Args pArgs)
{
	CC_ShowCommands(nullptr);
}
ADDCONSOLECOMMAND("Help", CC_Help)

void CC_Manager::AddCommand(CC_Info Info)
{
	const int NumCommands = CC_Manager::CommandInfo.size();
	if (NumCommands == 0)
	{
		CommandInfo.push_back(Info);
		return;
	}
	for (int i = 0; i < NumCommands; i++)
	{
		if (Info.Name.compare(CommandInfo[i].Name) < 0)
		{
			CommandInfo.insert(CommandInfo.begin() + i, Info);
			return;
		}
	}
	CommandInfo.push_back(Info);
}

CC_Info & CC_Manager::GetCC_Info(string CommandName)
{
	const int NumCommands = CC_Manager::CommandInfo.size();
	for (int i = 0; i < NumCommands; i++)
	{
		if (CC_Manager::CommandInfo[i].Name == CommandName)
			return CC_Manager::CommandInfo[i];
	}
	return NULL_CC_INFO;
}

CC_Info & CC_Manager::GetInfoBuffer()
{
	static CC_Info InfoBuffer;
	return InfoBuffer;
}


