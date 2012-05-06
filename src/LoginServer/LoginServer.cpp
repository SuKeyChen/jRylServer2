#include "LoginServer.h"

#include "GlobalConfTable.h"
#include "AuthServerMgr.h"

#include "../Common/stdDefs.h"
#include "../Common/typedef.h"
#include "../Common/configuration/ConfLoad.h"
#include "../Common/network/SocketClient.h"
#include "../Common/network/SocketServer.h"
#include "../Common/Logger.h"

#include "../Common/network/windows/WindowsSocketServer.h"

#define __LOG Common::Logger::GetInstance_ptr()

LoginServer::LoginServer(std::vector<std::string>& args)
{
	
	Common::network::windows::WindowsSocketServer* a = new Common::network::windows::WindowsSocketServer();
	a->BindAndListen("localhost", 1808);
}

bool LoginServer::LoadConfig()
{
	Common::configuration::ConfLoad confGlobal;
    Common::configuration::ConfLoad confLoginServer;
	GlobalConfTable* gconf = GlobalConfTable::GetInstance_ptr();

    if (!confGlobal.LoadConfig(FILE_CONF_GLOBAL_INI))
	{
        __LOG->ShowError("Error Load ini file(%s)\n", FILE_CONF_GLOBAL_INI);
		return false;
	}
    if (!confLoginServer.LoadConfig(FILE_CONF_LOGINSERVER_INI))
	{
        __LOG->ShowError("Error Load ini file(%s)\n", FILE_CONF_LOGINSERVER_INI);
        return false;
    }

    //LoginServer params load
    {
        //Create Log file
        string fileDir;
        bool fileTrucate;
        if (!confLoginServer.GetBool("Log", "Truncate", fileTrucate))
		{
            __LOG->ShowError("Error load key LoginServer::Log:Truncate\n");
            return false;
        }
        if (!confLoginServer.GetString("Log", "File", fileDir))
		{
            __LOG->ShowError("Error load key LoginServer::Log:File\n");
            return false;
        }
        gconf->LogFile = __LOG->CreateLogFile(fileDir.c_str(), fileTrucate);
		__LOG->AddObserver(gconf->LogFile);

		//Launcher
		if (!confLoginServer.GetUInt("Launcher", "PatchVersion", gconf->PatchVersion))
		{
            __LOG->ShowError("Error load key LoginServer::Launcher:Version\n");
            return false;
        }
        if (!confLoginServer.GetString("Launcher", "PatchAddress", gconf->PatchAddress))
		{
            __LOG->ShowError("Error load key LoginServer::Launcher:PatchAddress\n");
            return false;
        }

		if (!confLoginServer.GetString("LoginServer", "LauncherAddress", gconf->LauncherListenAddress))
		{
            __LOG->ShowError("Error load key load LoginServer::LoginServer:LauncherAddress\n");
            return false;
        }
		if (!confLoginServer.GetString("LoginServer", "LauncherPort", gconf->LauncherListenPort))
		{
            __LOG->ShowError("Error load key LoginServer::LoginServer:LauncherPort\n");
            return false;
        }

		//AuthServer
		if (!confLoginServer.GetString("LoginServer", "AuthServerAddress", gconf->AuthServerListenAddress))
		{
            __LOG->ShowError("Error load load key LoginServer::LoginServer:AuthServerAddress\n");
            return false;
        }
        if (!confLoginServer.GetString("LoginServer", "AuthServerPort",gconf->AuthServerListenPort))
		{
            __LOG->ShowError("Error load load key LoginServer::LoginServer:AuthServerPort\n");
            return false;
        }

    }

    //Global params load
    {
		if (!confGlobal.GetString("Server", "GlobalUser", gconf->GlobalUser))
		{
            __LOG->ShowError("Error load key Global::Server:GlobalUser\n");
            return false;
        }
		if (!confGlobal.GetString("Server", "GlobalPass", gconf->GlobalPass))
		{
            __LOG->ShowError("Error load key Global::Server:GlobalPass\n");
            return false;
        }
        
        byte GGkey[100];
        byte GGClientSeedKeyIni[100];
        byte GGServerSeedKeyIni[100];
        if (confGlobal.GetBytes("GameGuard", "Key", GGkey) < 0)
		{
            __LOG->ShowError("Error load key Global::GameGuard:Key\n");
            return false;
        }
        if (confGlobal.GetBytes("GameGuard", "ClientSeedKeyIni", GGClientSeedKeyIni) < 0)
		{
            __LOG->ShowError("Error load key Global::GameGuard:ClientSeedKeyIni\n");
            return false;
        }
        if (confGlobal.GetBytes("GameGuard", "ServerSeedKeyIni", GGServerSeedKeyIni) < 0)
		{
            __LOG->ShowError("Error load key Global::GameGuard:ServerSeedKeyIni\n");
            return false;
        }
		Common::crypto::CryptEngine::GetInstance().SetGGCryptParams(GGClientSeedKeyIni, GGServerSeedKeyIni, GGkey);
    }
	return true;
}

int LoginServer::Start()
{	
	m_authServer = new AuthServerMgr();
	m_authServer->Start();
	getchar();
	return 0;
}

void LoginServer::Stop()
{
	delete m_authServer;
}

LoginServer::~LoginServer()
{
	GlobalConfTable::DestroyInstance();
}