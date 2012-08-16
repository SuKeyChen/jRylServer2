#ifndef _LOGINSERVER_H_
#define _LOGINSERVER_H_

#include "../Common/stdDefs.h"
#include "../Common/typedef.h"
#include "../Common/Logger.h"
#include "../Common/iStartupClass.h"

#include <vector>
#include <string>

class AuthServerMgr;
class LauncherServer;

class LoginServer: public  Common::StartupClass {
public:
	LoginServer(std::vector<std::string>& args);
	~LoginServer();
	virtual bool LoadConfig();
    virtual int Start();
    virtual void Stop();
private:
	AuthServerMgr* m_authServer;
	LauncherServer* m_launcherServer;
};

#define STARTUP_CLASS LoginServer
#define MODULE_NAME "LoginServer"
#include "../Common/main.h"

#endif