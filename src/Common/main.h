#ifndef MAIN_H
#define MAIN_H

#if defined(_WIN32) || defined(_WIN64) 
#include <winsock2.h>
#include <windows.h>
#ifdef _DEBUG
#include <vld.h>
# endif
#else
# include <signal.h>
#endif 

#include "Logger.h"
#include "crypto/CryptEngine.h"
#include "Splash.h"

#include <string>
#include <vector>

using namespace Common;

bool StartSignalCtrl();

#ifdef STARTUP_CLASS

int main(int argc, char *argv[]) {

	Logger::GetInstance().ClearDefaultLogFile();
    Logger::GetInstance().ShowMessage(Splash, MODULE_NAME, 0);

	Logger::GetInstance().ShowInfo("[main] Creating module\n");
    std::vector<std::string> params;
    for (int i = 0; i < argc; i++) {
        params.push_back(argv[i]);
    }
    STARTUP_CLASS Main(params);

    Logger::GetInstance().ShowInfo("[main] Starting module configuration\n");
    if (!Main.LoadConfig()) {
        return -1;
    }

    Logger::GetInstance().ShowInfo("[main] Start module\n");
	int startRet = Main.Start();
	Main.Stop();

	Logger::GetInstance().ShowMessage(CL_RESET);
	Logger::DestroyInstance();
	crypto::CryptEngine::DestroyInstance();

    return startRet;
}
#else
#error STARTUP_CLASS not defined
#endif

#endif
