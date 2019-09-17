#pragma once
#include "utils.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace filesystem = std::experimental::filesystem;

void setDirectoryPermissionReadOnly(std::string dirPath);

void chromeRemoveExtensionInstallForceList();

void chromeRemoveAllPolicyAndLock();

void chromeRemoveExtensionInstallForceListSpecific(std::string ExtID);

std::string chromeGetExtensionDirectory();

void chromeRemoveInstalledExtension();

void chromePreventAdditionalExtensionInstall();
