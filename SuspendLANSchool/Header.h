#pragma once
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

extern bool isLanSchoolSuspended;

void getCurrentUsername(char* username);
void getLoggedOnUsername(char* username);
std::string getLoggedOnUsername();

bool checkIfNtAuthority();
void execAsNtAuthority();

void printErrorOpenProc(bool exitProg = false);

int getProcID(const char* procName);

void suspendProcThread(DWORD pid);
void resumeProcThread(DWORD pid);
void killProcess(DWORD pid);
