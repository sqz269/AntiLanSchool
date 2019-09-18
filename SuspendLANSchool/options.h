#pragma once
#include "utils.h"
#include "misc.h"
#include "antiWebController.h"

static const char* LANSCHOOLPROCESSNAME = "student.exe";

extern bool isLanSchoolSuspended;

void killLanSchool();

void suspendLanSchool();

void killProcessWrapper();

void disableKeyLogger();

void deleteLanSchoolChromeExt();

void launchExplorerWithPathWrapper();
