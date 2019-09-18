#define NOMINMAX
#define _CRT_SESCURE_NO_WARNINGS

#include "antiLanSchoolProcess.h"
#include "antiWebController.h"
#include <limits>


void displayOptions()
{
	puts("Choose options:\n");
	puts("0 - Kill LanSchool (students.exe)");
	if (isLanSchoolSuspended)
		puts("1 - Resume LanSchool");
	else
		puts("1 - Suspend LanSchool");
	puts("2 - Kill Process");
}

void parseOptions()
{
	int option;
	do
	{
		printf("Enter options: ");
		std::cin >> option;
	} while (std::cin.fail());
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	switch (option)
	{
	case (0):
		killProcess(getProcID(LANSCHOOLPROCESSNAME));

	case (1):
		if (isLanSchoolSuspended)
			resumeProcThread(getProcID(LANSCHOOLPROCESSNAME));
		else
			suspendProcThread(getProcID(LANSCHOOLPROCESSNAME));

	case (2):
	{
		printf("Enter target process name: ");
		std::string procname;
		std::getline(std::cin, procname);
		killProcess(getProcID(procname.c_str()));
	}

	default:
		break;
	}
}

int main()
{
	if (!checkIfNtAuthority())
		execAsNtAuthority();

	if (checkIfNtAuthority())
		setFilePermission("E:/DiskImages", DENY_ACCESS, 0xFFFFFFFF, SECURITY_WORLD_SID_AUTHORITY);
	getchar();
	//	chromeRemoveInstalledExtension();
	//while (true)
	//{
	//	displayOptions();
	//	parseOptions();
	//}
	//system("")
	//getUserAppdataDirectory();
}
