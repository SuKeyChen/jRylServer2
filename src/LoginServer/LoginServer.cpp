#include "LoginServer.h"

#define STARTUP_CLASS LoginServer
#define MODULE_NAME "LoginServer"
#include "../Common/main.h"

#include "../Common/packet/pktUserLogin.h"
#include "../Common/network/SocketClient.h"
#include "../Common/network/SocketServer.h"
#include "../Common/packet/pktPing.h"

LoginServer::LoginServer()
{
}

LoginServer::~LoginServer()
{

}

void LoginServer::initialize(Poco::Util::Application& self)
{
}

void LoginServer::uninitialize()
{
}

int LoginServer::main(const std::vector<std::string>& args)
{	
	waitForTerminationRequest();
	return 0;
}