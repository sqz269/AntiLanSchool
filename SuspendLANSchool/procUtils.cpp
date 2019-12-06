#include "procUtils.h"

/*

*/
void printErrorOpenProc()
{
	printf("call OpenProcess failed; function failed with error no: %d ;\n", GetLastError());
	printf("Press Enter to continue\n");
	getchar();
}

/*
	Get Process ID from Process Name

	@param procName - the process name which it's process id will be retrieved
*/
int getProcID(const char* procName)
{
	DWORD errorNo;
	HANDLE procList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (procList == INVALID_HANDLE_VALUE)
	{
		errorNo = GetLastError();
		printf("call CreateToolhelp32Snapshot failed; Function failed with error no: %d\n", errorNo);
		return -1;
	}

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	printf("Finding process with name %s\n", procName);
	if (Process32First(procList, &procEntry))
	{
		do
		{
			//printf("Process name: %s\n", procEntry.szExeFile);
			if (!strcmp(procName, procEntry.szExeFile))  // TODO Make flexble
			{
				printf("Process with name: %s Found, with process ID: %d\n", procEntry.szExeFile, procEntry.th32ProcessID);
				return procEntry.th32ProcessID;
			}
		} while (Process32Next(procList, &procEntry));
	}
	printf("No process with name %s can be found\n", procName);
	CloseHandle(procList);
	return NULL;
}

void startProcess(std::string targetProc)
{
	printf("Starting process: %s\n", targetProc.c_str());
	STARTUPINFO startInfo;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));
	CreateProcess(NULL, const_cast<char*>(targetProc.c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo);
	printf("Process created\n");
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
}

/*
	Suspend a process by suspending all it's threads using SuspendThread
	It can be resumed by calling resumeProcThread.

	@param pid - the pid of the process it will suspend
*/
void suspendProcThread(DWORD pid)
{
	if (pid == NULL)
	{
		printf("SUSPEND: INVALID PROCESS ID;");
		return;
	}

	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);

	do
	{
		if (threadEntry.th32OwnerProcessID == pid)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);
			if (hThread == NULL)
			{
				printf("Failed to suspend Process with pid: %d\n", pid);
				printErrorOpenProc();
				return;
			}
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	printf("Suspended Process with pid: %d\n", pid);
	CloseHandle(hThreadSnapshot);
}

/*
	Resume a process that is suspended by suspendProcThread
	Warning: A dead lock could happen as the order of thread resumption is random

	@param pid - the pid of the process it will resume
*/
void resumeProcThread(DWORD pid)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);

	do
	{
		if (threadEntry.th32OwnerProcessID == pid)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);

			if (hThread == NULL)
			{
				printf("Failed to Resume Process with pid: %d\n", pid);
				printErrorOpenProc();
				return;
			}
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));
	printf("Resumed Process with pid: %d\n", pid);

	CloseHandle(hThreadSnapshot);
}

/*
	Terminates a process with exit code 0

	@param pid - the pid of the process it will terminate

	@return 0 if the function succeed,
			error code from GetLastError will be returned upon failure
*/
int killProcess(DWORD pid)
{
	HANDLE proc = OpenProcess(PROCESS_TERMINATE, false, pid);
	if (TerminateProcess(proc, 0))  // Terminate Process return non zero
	{
		printf("Successfully terminated Process with pid %d\n", pid);
		CloseHandle(proc);
		return 0;
	}
	else
	{
		printf("Failed to terminate process with pid %d, ErrorNo: %d\n", pid, GetLastError());
		CloseHandle(proc);
		return GetLastError();
	}
}
