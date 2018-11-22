#pragma once

#include "CryEntitySystem\IEntity.h"



class CFileManager
{
public:

#ifdef _WIN32
#define SLASH_SYS "\\"
#else
#define SLASH_SYS ""
#endif

#define SLASH_CRY "/"

	static string m_FullRootPath_Sys;
	static string m_FullRootPath_Cry;

	static string m_AssetFolder;
	static string GetAssetFolderPath(bool bCry = true);

	static std::vector<string> GetFilesInDir(string DirName, bool bRelative = true);

};
