#pragma once
#include "utils.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace filesystem = std::experimental::filesystem;

void changeDirectoryPermissionDenyAll(std::string dirPath);

void changeDirectoryPermissionAllowAll(std::string dirPath);

void changeRegistryPermissionDenyAll(std::string registryPath);

void changeRegistryPermissionAllowAll(std::string registryPath);

void chromeRemoveExtensionInstallForceList();

void chromeRemoveAllPolicyAndLock();

void chromeRemoveExtensionInstallForceListSpecific(std::string ExtID);

std::wstring chromeGetExtensionDirectory();

void chromeRemoveInstalledExtension();

void chromePreventAdditionalExtensionInstall();
