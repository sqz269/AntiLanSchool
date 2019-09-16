#include "antiWebController.h"


void changeDirectoryPermissionDenyAll(std::string dirPath)
{

}

void changeDirectoryPermissionAllowAll(std::string dirPath)
{

}

void changeRegistryPermissionDenyAll(std::string registryPath)
{

}

void changeRegistryPermissionAllowAll(std::string registryPath)
{

}

void chromeRemoveExtensionInstallForceList()
{

}

/*
	Remove chrome policy enforced by removing every entry under
	HKEY_LOCAL_MACHINE/SOFTWARE/Policies/Google/Chrome/
	and then change the registry entry to read only
*/
void chromeRemoveAllPolicyAndLock()
{

}

/*
	Remove an extension from chrome policy: ExtensionInstallForcelist
	by editing the windows registry and deleting the key
*/
void chromeRemoveExtensionInstallForceListSpecific(std::string ExtID)
{

}

/*
	Get the path that stores packed extension for chrome
	it's usually %APPDATA%/Local/Google/Chrome/User Data/Default/Extension
*/
std::wstring chromeGetExtensionDirectory()
{
	std::wstring localAppdata = getUserLocalAppdataDirectory();
	return localAppdata;
}

/*
	Remove the current packed extension installed on chrome by deleting the extension folder
	Got from chromeGetExtensionDirectory
*/
void chromeRemoveInstalledExtension()
{

	for (filesystem::directory_entry file : filesystem::directory_iterator(chromeGetExtensionDirectory()))
	{
		std::wcout << L"Removing: " << file.path().c_str();
		filesystem::remove_all(file);
	}
}

/*
	Prevents chrome from installing additional extension from web store
	by setting the packed extension's directory to read only
*/
void chromePreventAdditionalExtensionInstall()
{

}
