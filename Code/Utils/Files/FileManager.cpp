#include "StdAfx.h"
#include "FileManager.h"

#ifdef WIN32
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")
#endif // WIN32

#ifdef _WIN32
string CFileManager::m_FullRootPath_Sys = "D:\\Desktop\\Cryengine Stuff\\Engine Files\\Projects\\Cry_FirstPerson";
#else
string CFileManager::m_FullRootPath_Sys = "";
#endif

string CFileManager::m_FullRootPath_Cry = "D:/Desktop/Cryengine Stuff/Engine Files/Projects/Cry_FirstPerson";
string CFileManager::m_AssetFolder = "Assets";

string CFileManager::GetAssetFolderPath(bool bCry)
{
	if (bCry)
		return m_FullRootPath_Cry + SLASH_CRY + m_AssetFolder;
	else
		return m_FullRootPath_Sys + SLASH_SYS + m_AssetFolder;
}

#ifdef _WIN32
std::vector<string> CFileManager::GetFilesInDir(string DirName, bool bRelative)
{
	std::vector<string> Files;

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	if (bRelative)
		DirName = m_FullRootPath_Sys + "\\" + DirName;

	bool Error = false;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(DirName, MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3)) {
		gEnv->pLog->LogError("Directory path is too long");
		Error = true;
		return Files;
	}
	
	gEnv->pLog->Log("Target directory is: " + DirName);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, DirName);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		gEnv->pLog->LogError("Error In: FindFirstFile");
		Error = true;
		return Files;
	}

	// List all the files in the directory with some info about them.

	do {

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			//Files.push_back((string)ffd.cFileName);

		}
		else {

			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			Files.push_back(ffd.cFileName);

		}

	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		gEnv->pLog->LogError("Error In: FindFirstFile");
		Error = true;
	}

	FindClose(hFind);

	return Files;
}
#else
std::vector<string> CFileManager::GetFilesInDir(string DirName, bool bRelative)
{
	std::vector<string> Files;

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	if (bRelative)
		DirName = m_FullRootPath_Sys + "\\" + DirName;

	bool Error = false;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(DirName, MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3)) {

		Error = true;

	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, DirName);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		Error = true;
	}

	// List all the files in the directory with some info about them.

	do {

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			//Files.push_back((string)ffd.cFileName);

		}
		else {

			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			Files.push_back(ffd.cFileName);

		}

	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {

		Error = true;

	}

	FindClose(hFind);

	return Files;
}
#endif // _WIN32

