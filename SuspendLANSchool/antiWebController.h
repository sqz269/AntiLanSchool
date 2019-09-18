#pragma once
#include "utils.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace filesystem = std::experimental::filesystem;

void setDirectoryPermissionDenyAll(std::string dirPath);

void setRegKeyPermissionDenyAll(std::string registryPath);

void chromeRemoveExtensionInstallForceList();

void chromeRemoveAllPolicyAndLock();

std::string chromeRemoveInstalledExtensionSpecific(std::string ExtID);

std::string chromeGetExtensionDirectory();

void chromeRemoveInstalledExtension();

void chromePreventExtensionInstall(std::string extId);
