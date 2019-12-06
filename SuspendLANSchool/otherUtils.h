#pragma once
#include "procUtils.h"

#define MAX_NAME 256

const static ACCESS_MASK PERMISSION_ALL_PERMISSION = 0xFFFFFFFF;

void setFilePermission(LPCTSTR fileName, _ACCESS_MODE accessMode, ACCESS_MASK accessPermissions, SID_IDENTIFIER_AUTHORITY permissionTargetEntity);

std::string getUserLocalAppdataDirectory();

bool checkIfNtAuthority();
void execAsNtAuthority();
