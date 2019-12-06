#include "otherUtils.h"

/*
	set a file to desired permission
	https://stackoverflow.com/questions/910528/how-to-change-the-acls-from-c

	@param - fileName path to the object that's permission needs to be changed
	@param - accessMode: https://docs.microsoft.com/en-us/windows/win32/api/accctrl/ne-accctrl-access_mode
	@param - accessPermissions: https://docs.microsoft.com/en-us/windows/win32/secauthz/access-mask
	@param - permissionTargetEntity: https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/c6ce4275-3d90-4890-ab3a-514745e4637e
*/
void setFilePermission(LPCTSTR fileName, _ACCESS_MODE accessMode, ACCESS_MASK accessPermissions, SID_IDENTIFIER_AUTHORITY permissionTargetEntity)
{
	PSID pEntitySID = NULL;
	PACL pACL = NULL;
	EXPLICIT_ACCESS ea[1];
	SID_IDENTIFIER_AUTHORITY SIDAuthEntity = permissionTargetEntity;

	// Create a well-known SID for the Everyone group.
	AllocateAndInitializeSid(&SIDAuthEntity, 1,
		SECURITY_WORLD_RID,
		0, 0, 0, 0, 0, 0, 0,
		&pEntitySID);

	// Initialize an EXPLICIT_ACCESS structure for an ACE.
	ZeroMemory(&ea, 1 * sizeof(EXPLICIT_ACCESS));
	ea[0].grfAccessPermissions = accessPermissions;
	ea[0].grfAccessMode = accessMode;
	ea[0].grfInheritance = NO_INHERITANCE;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName = (LPTSTR)pEntitySID;

	// Create a new ACL that contains the new ACEs.
	SetEntriesInAcl(1, ea, NULL, &pACL);

	// Initialize a security descriptor.  
	PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
		SECURITY_DESCRIPTOR_MIN_LENGTH);

	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);

	// Add the ACL to the security descriptor. 
	SetSecurityDescriptorDacl(pSD,
		TRUE,     // bDaclPresent flag   
		pACL,
		FALSE);   // not a default DACL 



//Change the security attributes
	SetFileSecurity(fileName, DACL_SECURITY_INFORMATION, pSD);

	std::cout << "Successfully Set ACL Attributes" << std::endl;

	if (pEntitySID)
		FreeSid(pEntitySID);
	if (pACL)
		LocalFree(pACL);
	if (pSD)
		LocalFree(pSD);
}

//void setRegistryEntryPermission(HKEY key, LPCSTR subkey)
//{
//	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
//		TEXT("SOFTWARE\\Microsoft\\DirectShow\\Preferred"),
//		0,
//		KEY_READ,
//		&hTestKey) == ERROR_SUCCESS
//	)
//	{
//		while (QueryKey(hTestKey));
//	}
//}

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
HRESULT GetUserFromProcess(const DWORD procId, std::string& strUser, std::string& strdomain)
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
	std::string localAppData = "C:\\Users\\" + user + "\\AppData\\Local";
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
