#pragma once
#include "otherUtils.h"
#include "procUtils.h"
#include "antiWebController.h"

static const char* LANSCHOOLPROCESSNAME = "procexp64.exe";

extern bool isLanSchoolSuspended;

void killLanSchool();

void suspendLanSchool();

void killProcessWrapper();

void disableKeyLogger();

void deleteLanSchoolChromeExt();

void launchExplorerWithPathWrapper();
