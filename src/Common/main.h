#ifndef MAIN_H
#define MAIN_H
#include <string>
#include <vector>

#include "Logger.h"
#include "Splash.h"

using namespace Common;

#if defined(_WIN32) || defined(_WIN64) 
#include <windows.h>
#ifdef _DEBUG
#include <vld.h>
# endif
#else
# include <signal.h>
#endif

bool StartSignalCtrl();

#ifdef STARTUP_CLASS

int main(int argc, char *argv[]) {
    Logger::GetInstance().ClearDefaultLogFile();
    Logger::GetInstance().ShowMessage(Splash, MODULE_NAME, 0);

	int startRet;
	STARTUP_CLASS app;
    startRet = app.run(argc, argv);
    
	Logger::GetInstance().ShowMessage(CL_RESET);
	Logger::DestroyInstance();
    
    return startRet;
}
#else
#error STARTUP_CLASS not defined
#endif

#endif
