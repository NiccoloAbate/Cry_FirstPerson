#include "StdAfx.h"
#include "AudioController.h"

#include <CryAudio\IAudioSystem.h>

#include "Utils\ConsoleCommands.h"

void CC_TestAudio(CC_Args pArgs)
{
	const string Name = "Game 03";
	CAudioController::PlayMusic(Name);
}
ADDCONSOLECOMMAND("a_Test", CC_TestAudio)
void CC_MuteAudio(CC_Args pArgs)
{
	CAudioController::MuteAll();
}
ADDCONSOLECOMMAND_WITHINFOBUFFER(CC_MuteAudio)
CC_SETINFOBUFFER("a_Mute", 0)
void CC_SetAudioVolume(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1)
	CC_GETARG(float, Volume, 1)
	const CryAudio::ControlId ParamId = CryAudio::StringToId("volume_sfx");
	gEnv->pAudioSystem->SetParameter(ParamId, Volume);
}
CC_Info _g_CC_SetAudioVolumeInfo = CC_Info("a_SetVolume", 1);
ADDCONSOLECOMMAND_WITHINFOCLASS(_g_CC_SetAudioVolumeInfo, CC_SetAudioVolume)

void CAudioController::PlayMusic(string TriggerName)
{
	const string Name = TriggerName;
	const CryAudio::ControlId Id = CryAudio::StringToId(Name.c_str());
	const CryAudio::EOcclusionType OclusionType = CryAudio::EOcclusionType::Ignore;
	const CryAudio::CObjectTransformation Transformation = Vec3(0, 0, 0);
	const CryAudio::SExecuteTriggerData Data = CryAudio::SExecuteTriggerData(Name.c_str(), OclusionType, Transformation, false, Id);
	gEnv->pAudioSystem->ExecuteTriggerEx(Data);
}

void CAudioController::MuteAll()
{
	static bool bMuted = false;
	if (bMuted)
	{
		gEnv->pAudioSystem->UnmuteAll();
	}
	else
	{
		gEnv->pAudioSystem->MuteAll();
	}
	bMuted = !bMuted;
}
