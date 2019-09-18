#include "misc.h"

void startExplorerAtPath(std::string targetPath)
{
	targetPath = "explorer.exe " + targetPath;
	STARTUPINFO startInfo;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));
	CreateProcess(NULL, const_cast<char*>(targetPath.c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo);
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
}

