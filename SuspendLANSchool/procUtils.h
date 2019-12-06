#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <ShlObj_core.h>
#include <comdef.h>
#include <sddl.h>
#include <AccCtrl.h>
#include <AclAPI.h>
#include <string>

HRESULT GetUserFromProcess(const DWORD procId, std::string& strUser, std::string& strdomain);
BOOL GetLogonFromToken(HANDLE hToken, std::string& strUser, std::string& strdomain);

int getProcID(const char* procName);

void startProcess(std::string procName);

void printErrorOpenProc();

void suspendProcThread(DWORD pid);
void resumeProcThread(DWORD pid);
int killProcess(DWORD pid);
