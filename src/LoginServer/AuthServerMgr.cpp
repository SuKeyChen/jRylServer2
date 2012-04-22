#include "AuthServerMgr.h"

#include "../Common/typedef.h"
#include "../Common/stdDefs.h"
#include "../Common/Logger.h"

#include "GlobalConfTable.h"

#include <boost/bind.hpp>

#include <ctime>

#define __LOG Common::Logger::GetInstance_ptr()

AuthServerMgr::AuthServerMgr()
{
	
	for(int i = 0;i < AUTHSERVER_SLOT_LENGTH; i++)
	{
		m_authServers[i] = NULL;
	}
}
bool AuthServerMgr::Start()
{
	GlobalConfTable* gconf = GlobalConfTable::GetInstance_ptr();

	m_socketListen.SetAcceptCallback(boost::bind(&AuthServerMgr::ClientConnect, this, _1));
	if(!m_socketListen.BindAndListen(gconf->AuthServerListenAddress, gconf->AuthServerListenPort))
	{
		__LOG->ShowError("Error start bind and listen address for AuthServers\n");
		return false;
	}
	return true;
}

void AuthServerMgr::ClientConnect(Common::network::SocketClient* client)
{
	m_unauthorizedAuthServers[client].AttemptLogin = 0;
	m_unauthorizedAuthServers[client].State = UnauthorizedAuthServer::US_WAIT_PING;
	m_unauthorizedAuthServers[client].Time = ::time(NULL);
	client->SetCloseCallback(boost::bind(&AuthServerMgr::ClientDesconnect, this, _1));
	client->SetReceivePacketCallback(boost::bind(&AuthServerMgr::PacketParserUnauthorized, this, _1, _2));
}

void AuthServerMgr::PacketParserUnauthorized(Common::network::SocketClient* client, Common::Buffer_ptr buff)
{

}

void AuthServerMgr::ClientDesconnect(Common::network::SocketClient* client)
{
	m_unauthorizedAuthServers.erase(client);
	delete client;
}

AuthServerMgr::~AuthServerMgr()
{
	std::map<Common::network::SocketClient*, UnauthorizedAuthServer>::iterator it = m_unauthorizedAuthServers.begin();
	while(it != m_unauthorizedAuthServers.end())
	{
		delete it->first;
		it++;
	}
	for(int i = 0;i < AUTHSERVER_SLOT_LENGTH; i++)
	{
		if(m_authServers[i] != NULL)
		{
			delete m_authServers[i];
		}
	}
}