#include "utils.h"

/*
	Checks if the program is running as Nt Authority/System
*/
bool checkIfNtAuthority()
{
	char user[128];
	DWORD user_len = 128;
	GetUserName(user, &user_len);
	return !strcmp(user, "SYSTEM");
}

/*https://stackoverflow.com/questions/2686096/c-get-username-from-process/2686150#2686150
*/
BOOL GetLogonFromToken(HANDLE hToken, std::string& strUser, std::string& strdomain)
{
	DWORD dwSize = MAX_NAME;
	BOOL bSuccess = FALSE;
	DWORD dwLength = 0;
	strUser = "";
	strdomain = "";
	PTOKEN_USER ptu = NULL;
	//Verify the parameter passed in is not NULL.
	if (NULL == hToken)
		goto Cleanup;

	if (!GetTokenInformation(
		hToken,         // handle to the access token
		TokenUser,    // get information about the token's groups 
		(LPVOID)ptu,   // pointer to PTOKEN_USER buffer
		0,              // size of buffer
		&dwLength       // receives required buffer size
	))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			goto Cleanup;

		ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, dwLength);

		if (ptu == NULL)
			goto Cleanup;
	}

	if (!GetTokenInformation(
		hToken,         // handle to the access token
		TokenUser,    // get information about the token's groups 
		(LPVOID)ptu,   // pointer to PTOKEN_USER buffer
		dwLength,       // size of buffer
		&dwLength       // receives required buffer size
	))
	{
		goto Cleanup;
	}
	SID_NAME_USE SidType;
	char lpName[MAX_NAME];
	char lpDomain[MAX_NAME];

	if (!LookupAccountSid(NULL, ptu->User.Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
	{
		DWORD dwResult = GetLastError();
		if (dwResult == ERROR_NONE_MAPPED)
			strcpy(lpName, "NONE_MAPPED");
		else
		{
			printf("LookupAccountSid Error %u\n", GetLastError());
		}
	}
	else
	{
		printf("Current user is  %s\\%s\n",
			lpDomain, lpName);
		strUser = std::string(lpName);
		strdomain = std::string(lpDomain);
		bSuccess = TRUE;
	}

Cleanup:

	if (ptu != NULL)
		HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);
	return bSuccess;
}

/*https://stackoverflow.com/questions/2686096/c-get-username-from-process/2686150#2686150
*/
HRESULT GetUserFromProcess(const DWORD procId, std::string &strUser, std::string &strdomain)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procId);
	if (hProcess == NULL)
		return E_FAIL;
	HANDLE hToken = NULL;

	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
	{
		CloseHandle(hProcess);
		return E_FAIL;
	}
	BOOL bres = GetLogonFromToken(hToken, strUser, strdomain);

	CloseHandle(hToken);
	CloseHandle(hProcess);
	return bres ? S_OK : E_FAIL;
}
/*
	@Deprecated
*/
std::string getUserLocalAppdataDirectory()
{
	std::string user;
	std::string domain;
	GetUserFromProcess(getProcID("explorer.exe"), user, domain);
	std::string localAppData = "C:\\Users\\" + user+ "\\AppData\\Local";
	return localAppData;
}

/*
	Re-execute current program as Nt Authority/System through Psexec
*/
void execAsNtAuthority()
{
	printf("Starting procedure to elevate to [NT Authority/System] Privilege.\n");
	char exePath[MAX_PATH];

	HMODULE thisModule = GetModuleHandle(NULL);  // Get current executable path

	if (thisModule == NULL)
	{
		// TODO Allow user to enter exe path
		printf("Unable to get current process's handle; failed with error no: %d\n", GetLastError());
		printf("Press Enter to exit");
		getchar();
		exit(1);
	}
	else
	{
		GetModuleFileName(thisModule, exePath, MAX_PATH);
		printf("Got Executable Path: %s\n", exePath);
	}
	std::string psexecPath;
	printf("Enter PsExec.exe/PsExec64.exe path (Including File Name): ");
	//std::getline(std::cin, psexecPath);
	std::cin >> psexecPath;

	if (psexecPath.empty())
	{
		psexecPath = "S:/ShellScript/PsExec64.exe"; // Default path for psexec
		printf("PsExec.exe path not specified, using default: %s\n", psexecPath.c_str());
	}

	std::string command;
	command = psexecPath + " -i -s " + exePath;
	printf("Constructed command: %s\n", command.c_str());
	printf("Executing Command...");

	//std::thread::
	system(command.c_str());
	getchar();
	//exit(0);
}

/*

*/
void printErrorOpenProc()
{
	printf("call OpenProcess failed; function failed with error no: %d ;\n", GetLastError());
	printf("Press Enter to continue");
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
	return -1;
}

/*
	Suspend a process by suspending all it's threads using SuspendThread
	It can be resumed by calling resumeProcThread.

	@param pid - the pid of the process it will suspend
*/
void suspendProcThread(DWORD pid)
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
				printf("Failed to suspend Process with pid: %d", pid);
				printErrorOpenProc();
				return;
			}
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	printf("Suspended Process with pid: %d", pid);
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
				printf("Failed to Resume Process with pid: %d", pid);
				printErrorOpenProc();
				return;
			}
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));
	printf("Resumed Process with pid: %d", pid);

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
		printf("Successfully terminated Process with pid %d", pid);
		CloseHandle(proc);
		return 0;
	}
	else
	{
		printf("Failed to terminate process with pid %d, ErrorNo: %d", pid, GetLastError());
		CloseHandle(proc);
		return GetLastError();
	}
}
