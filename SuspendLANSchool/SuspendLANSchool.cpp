#define NOMINMAX
#define DEBUG_MODE 0

#include "options.h"
#include "otherUtils.h"
#include <limits>


void displayOptions()
{
	puts("Currently running as [NT Authority\\System]");
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
	int option = input<int>("Enter options: ");

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
	puts("2 - Remove Specific Chrome plugin");
	puts("3 - Make Chrome Extension Directory Read Only");
	puts("4 - Open explorer to specific path (Bypass C: Drive hiding)");
}

void parseOptionsNoPrivilege()
{
	int option = input<int>("Enter Options: ");

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

		break;
	}
	case (3):
	{
		setDirectoryPermissionDenyAll(chromeGetExtensionDirectory());
		break;
	}

	case (4):
	{
		launchExplorerWithPathWrapper();
		break;
	}

	default:
		break;
	}
}

int main()
{
	if (DEBUG_MODE)
	{
		
	}
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
