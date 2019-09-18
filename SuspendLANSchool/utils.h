#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <ShlObj_core.h>
#include <comdef.h>

#include <AccCtrl.h>
#include <AclAPI.h>

#include <thread>
#include <string>

#define MAX_NAME 256

struct PermCtrlSID
{

};

void setFilePermission(LPCTSTR fileName, _ACCESS_MODE accessMode, unsigned long accessPermissions, SID_IDENTIFIER_AUTHORITY permissionTargetEntity);

HRESULT GetUserFromProcess(const DWORD procId, std::string& strUser, std::string& strdomain);
BOOL GetLogonFromToken(HANDLE hToken, std::string& strUser, std::string& strdomain);

std::string getUserLocalAppdataDirectory();

bool checkIfNtAuthority();
void execAsNtAuthority();

void printErrorOpenProc();

int getProcID(const char* procName);

void suspendProcThread(DWORD pid);
void resumeProcThread(DWORD pid);
int killProcess(DWORD pid);
