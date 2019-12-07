#pragma once
#include "procUtils.h"
#define NOMINMAX
#define MAX_NAME 256

const static ACCESS_MASK PERMISSION_ALL_PERMISSION = 0xFFFFFFFF;

void setFilePermission(LPCTSTR fileName, _ACCESS_MODE accessMode, ACCESS_MASK accessPermissions, SID_IDENTIFIER_AUTHORITY permissionTargetEntity);

std::string getUserLocalAppdataDirectory();

bool checkIfNtAuthority();
void execAsNtAuthority();

template <class T>
T input(std::string message)
{
	printf("%s", message.c_str());
	T userInput;
	std::cin >> userInput;
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		printf("%s", message.c_str());
		std::cin >> userInput;
	};
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	return userInput;
}
