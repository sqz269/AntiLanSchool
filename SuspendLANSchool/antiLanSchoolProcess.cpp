#include "antiLanSchoolProcess.h"

bool isLanSchoolSuspended = 1;

void killLanSchool()
{
	int lanSchoolPid = getProcID(LANSCHOOLPROCESSNAME);
	if (lanSchoolPid == -1)
	{
		printf("Kill: Failed to find %s's process id.", LANSCHOOLPROCESSNAME);
		return;
	}
	
	int status = killProcess(lanSchoolPid);
	if (!status)  // killProcess will return 0 if no error
		printf("%s is successfully terminated;", LANSCHOOLPROCESSNAME);
	else
		printf("Failed to kill %s. function failed with error: %d", LANSCHOOLPROCESSNAME, status);
}

void suspendLanSchool()
{
	int lanSchoolPid = getProcID(LANSCHOOLPROCESSNAME);
	if (lanSchoolPid == -1)
	{
		printf("Failed to get %s's Pid; ", LANSCHOOLPROCESSNAME);
		return;
	}

	if (isLanSchoolSuspended)
	{
		resumeProcThread(lanSchoolPid);
	}
	else
	{
		suspendProcThread(lanSchoolPid);
	}
}
