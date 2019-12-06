#pragma once
#include "otherUtils.h"
#include "procUtils.h"
#include <iostream>
#include <string>
#include <filesystem>

const static std::string chromeRegExtensionInstallBlackList = "Software\\Policies\\Google\\Chrome\\ExtensionInstallBlacklist";
const static std::string chromeRegExtensionForceList = "Software\\Policies\\Google\\Chrome\\ExtensionInstallForcelist";
const static std::string chromeRegPolicyRootEntry = "Software\\Policies\\Google\\Chrome";

namespace filesystem = std::filesystem;

void setDirectoryPermissionDenyAll(std::string dirPath);

void setRegKeyPermissionDenyAll(std::string registryPath);

void chromeRemoveExtensionInstallForceList();

void chromeRemoveAllPolicyAndLock();

std::string chromeRemoveInstalledExtensionSpecific(std::string ExtID);

std::string chromeGetExtensionDirectory();

void chromeRemoveInstalledExtension();

void chromePreventExtensionInstall(std::string extId);

