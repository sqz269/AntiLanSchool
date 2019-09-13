// SuspendLANSchool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#include <thread>
#include <string>


enum suspendType
{
	KILL,
	DEBUGGER,
	THREAD
};

/*
*/
bool checkIfNtAuthority()
{	
	char user[128];
	DWORD user_len = 128;
	GetUserName(user, &user_len);
	printf("Current user name: %s\n", user);
	return !strcmp(user, "SYSTEM");
}

void printErrorOpenProc(bool exitProg=false)
{
	printf("call OpenProcess failed; function failed with error no: %d ;\n", GetLastError());
	puts("Press Enter to continue");
	getchar();
	if (exitProg)
		exit(1);
}

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
				printErrorOpenProc();
				return;
			}
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	CloseHandle(hThreadSnapshot);
}

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
				printErrorOpenProc();
			}
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	CloseHandle(hThreadSnapshot);
}

void killProcess(DWORD pid)
{
	HANDLE proc = OpenProcess(PROCESS_TERMINATE, false, pid);
	if (proc == NULL)
	{
		printErrorOpenProc();
	}
	else if (!TerminateProcess(proc, 0))  // Terminate Process return a 0 if the operation failed
	{
		printf("Failed to terminate process with pid: %d; Failed with error code: %d\n", pid, GetLastError());
	}
	else
		printf("Process with pid: %d Killed;", pid);
	CloseHandle(proc);
}
	
//void suspendProcNtSuspendProc(DWORD pid)
//{
//	typedef LONG(NTAPI * NtSuspendProcess)(IN HANDLE ProcessHandle);
//	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
//
//	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
//		GetModuleHandle(L"ntdll"), "NtSuspendProcess");
//
//	pfnNtSuspendProcess(processHandle);
//	CloseHandle(processHandle);
//}

void procOps(DWORD pid, short suspendType=DEBUGGER)
{
	if (suspendType == DEBUGGER)
	{
		DebugActiveProcess(pid);
	}
	else if (suspendType == THREAD)
	{
		suspendProcThread(pid);
	}
	else if (suspendType == KILL)
	{
		killProcess(pid);
	}
}

void resumeProcess(DWORD pid, short suspendedMethod = DEBUGGER)
{
	if (suspendedMethod == DEBUGGER)
	{
		DebugActiveProcessStop(pid);
	}
	else
	{
		resumeProcThread(pid);
	}
}

int getProcID(const char * procName)
{
	DWORD errorNo;
	HANDLE procList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (procList == INVALID_HANDLE_VALUE)
	{
		errorNo = GetLastError();
		printf("call CreateToolhelp32Snapshot failed; Function failed with error no: %d\n", errorNo);
		abort();
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
	printf("Press enter to exit\n");
	std::cin.ignore();
	//exit(0);
	return 0;
}

void execAsNtAuthority()
{
	printf("Starting procedure to elevate to [NT Authority/System] Privilege.\n");
	char exePath[MAX_PATH];

	HMODULE thisModule = GetModuleHandle(NULL);

	if (thisModule == NULL)
	{
		// TODO Allow user to enter exe path
		printf("Unable to get current Executable path; failed with error no: %d\n", GetLastError());
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
		psexecPath = "S:/ShellScript/PsExec64.exe";
		printf("PsExec.exe path not specified, using default: %s\n", psexecPath.c_str());
	}
	std::string sExePath(exePath);
	std::string command;
	command = psexecPath + " -i -s " + exePath;
	printf("Constructed command: %s\n", command.c_str());
	printf("Executing Command...");

	system(command.c_str());

	exit(0);
}

void listenKey()
{

}

void displayOptions()
{

	printf("Choose an option:\n");
	if (!checkIfNtAuthority())
		printf("0 - Elevate privilege to NT AUTHORITY/SYSTEM\n");

	printf("1 - Kill LAN school (students.exe)\n");
	printf("2 - Suspend LAN school\n");
	printf("3 - Resume LAN school\n");
	printf("4 - Launch program with SYSTEM\n");
}

void parseOptions()
{
	int option;
	while (true)
	{
		printf("Enter choice: ");
		std::cin >> option;
		if (std::cin.fail())
		{
			continue;
		}
		break;
	}

	switch (option)
	{
	case 0:
		if (checkIfNtAuthority())
		{
			printf("Program is already running as NT AUTHORITY/SYSTEM");
			break;
		}
		else
		{
			execAsNtAuthority();
		}

	case 1:
		killProcess(getProcID("Taskmgr.exe"));

	case 2:
		procOps(getProcID("Taskmgr.exe"), THREAD);

	case 3:
		resumeProcThread(getProcID("Taskmgr.exe"));

	case 4:
		printf("work in progress");

	default:
		printf("Invalid options");
	}
}

int main()
{
	while (true)
	{
		displayOptions();
		parseOptions();
	}
}