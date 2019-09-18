#include "options.h"

bool isLanSchoolSuspended = false;

void killLanSchool()
{
	int lanSchoolPid = getProcID(LANSCHOOLPROCESSNAME);
	if (lanSchoolPid == NULL)
	{
		printf("Failed to find %s's process id.\n", LANSCHOOLPROCESSNAME);
		return;
	}
	
	killProcess(lanSchoolPid);
}

void suspendLanSchool()
{
	int lanSchoolPid = getProcID(LANSCHOOLPROCESSNAME);
	if (lanSchoolPid == NULL)
	{
		printf("Failed to get %s's Pid; ", LANSCHOOLPROCESSNAME);
		return;
	}

	if (isLanSchoolSuspended)
	{
		resumeProcThread(lanSchoolPid);
		isLanSchoolSuspended = false;
	}
	else
	{
		suspendProcThread(lanSchoolPid);
		isLanSchoolSuspended = true;
	}
}

void killProcessWrapper()
{
	printf("WARNING: Process names are case sensitive\nEnter target process name: ");
	std::string procName;
	std::getline(std::cin, procName);
	int procId = getProcID(procName.c_str());
	if (procId == NULL)
	{
		printf("Failed to get process id for process name: %s", procName.c_str());
	}
	else
		killProcess(procId);
}

void disableKeyLogger()
{

}

void deleteLanSchoolChromeExt()
{
	std::string lanSchoolExtId = "honjcnefekfnompampcpmcdadibmjhlk";
	std::string lanSchoolExtPath = chromeRemoveInstalledExtensionSpecific(lanSchoolExtId);
	if (lanSchoolExtPath != "")
	{
		printf("Successfully removed lanschool extension folder, now setting permissions\n");
		CreateDirectory(lanSchoolExtPath.c_str(), NULL);
		setDirectoryPermissionDenyAll(lanSchoolExtPath);
	}
	else
		printf("Failed to delete lanschool chrome extension.");
}


void launchExplorerWithPathWrapper()
{
	std::string path;
	printf("Path must be entered with back space. Ex: C:/Users/Test -> C:\\Users\\Test\n");
	printf("Enter Path open to (Default C:\\): ");
	std::getline(std::cin, path);
	if (path.empty())
	{
		path = "C:\\";
		printf("No path entered: Defaulting to C:\\");
	}
	startExplorerAtPath(path);
}
