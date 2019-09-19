#define NOMINMAX

#include "options.h"
#include "misc.h"
#include <limits>


void displayOptions()
{
	puts("Choose options:");
	puts("0 - Kill LanSchool (students.exe)");
	if (isLanSchoolSuspended)
		puts("1 - Resume LanSchool");
	else
		puts("1 - Suspend LanSchool");
	puts("2 - Kill Process");
	puts("3 - Disable LanSchool Key logger (lskdata1.bin)");
	puts("4 - Remove LanSchool Chrome Extension (Chrome Restart Required)");
	puts("5 - Make Chrome Extension Directory Read Only");
	puts("6 - Open explorer to specific path (Bypass C: Drive hiding)");
}

void parseOptions()
{
	int option;

	printf("Enter options: ");
	std::cin >> option;
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		printf("Enter options: ");
		std::cin >> option;
	};
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	printf("\n\n--------------------------------------\nExecution Details: \n");
	switch (option)
	{
		case (0):
		{
			killLanSchool();
			break;
		}

		case (1):
		{
			suspendLanSchool();
			break;
		}

		case (2):
		{
			killProcessWrapper();
			break;
		}

		case (3):
		{
			printf("Disable Key Logger feature work in progress\n");
			break;
		}

		case (4):
		{
			deleteLanSchoolChromeExt();
			break;
		}

		case (5):
		{
			setDirectoryPermissionDenyAll(chromeGetExtensionDirectory());
			break;
		}

		case (6):
		{
			launchExplorerWithPathWrapper();
			break;
		}

		default:
			break;
	}
}

void displayOptionsNoPrivilege()
{
	puts("Choose options:");
	puts("0 - Elevate to SYSTEM privilege (Program needs to be executed as admin first)");
	puts("1 - Remove LanSchool Chrome Extension (Chrome Restart Required)");
	puts("2 - Make Chrome Extension Directory Read Only");
	puts("3 - Open explorer to specific path (Bypass C: Drive hiding)");
}

void parseOptionsNoPrivilege()
{
	int option;

	printf("Enter options: ");
	std::cin >> option;
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		printf("Enter options: ");
		std::cin >> option;
	};
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	printf("\n\n--------------------------------------\nExecution Details: \n");
	switch (option)
	{
	case (0):
	{
		execAsNtAuthority();
		break;
	}

	case (1):
	{
		deleteLanSchoolChromeExt();
		break;
	}
	case (2):
	{
		setDirectoryPermissionDenyAll(chromeGetExtensionDirectory());
	}

	case (3):
	{
		launchExplorerWithPathWrapper();
	}

	default:
		break;
	}
}

int main()
{
	if (checkIfNtAuthority())
	{
		while (true)
		{
			displayOptions();
			parseOptions();
			printf("--------------------------------------\n\n");
		}
	}
	else
	{
		printf("Elevate to SYSTEM (option 0) to unlock more options\n");
		while (true)
		{
			displayOptionsNoPrivilege();
			parseOptionsNoPrivilege();
			printf("--------------------------------------\n\n");
		}
	}

	printf("Press Enter to exit");
	getchar();
	exit(0);
}
