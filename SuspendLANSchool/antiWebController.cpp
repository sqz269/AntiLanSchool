#include "antiWebController.h"

void setDirectoryPermissionReadOnly(std::string dirPath)
{
	// Set permission utilizing commands because windows API is lame
	
}

void setRegistryKeyPermissionReadOnly(std::string registryPath)
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
	std::string chromePolicyRegPath = "SOFTWARE\\Policies\\Google\\Chrome\\";
	RegDeleteKey(HKEY_LOCAL_MACHINE, chromePolicyRegPath.c_str());
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
std::string chromeGetExtensionDirectory()
{
	std::string localAppdata = getUserLocalAppdataDirectory();
	std::string extensionDirectory = localAppdata + "\\Google\\Chrome\\User Data\\Default\\Extensions";
	return extensionDirectory;
}

/*
	Remove a specific extension from the extension directory from extension id

	@param - extId the extension ID to remove
*/
std::string chromeRemoveInstalledExtensionSpecific(std::string extId)
{
	return "";
}

/*
	Remove the current packed extension installed on chrome by deleting the extension folder
	Got from chromeGetExtensionDirectory
*/
void chromeRemoveInstalledExtension()
{
	std::string dir = chromeGetExtensionDirectory();
	std::cout << "Target Directory: " << dir << std::endl;
	for (filesystem::directory_entry file : filesystem::directory_iterator(dir))
	{
		std::cout << "Removing: " << file.path().string() << std::endl;
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
