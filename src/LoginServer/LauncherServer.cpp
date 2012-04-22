#include "LauncherServer.h"

#include <boost/bind.hpp>

LauncherServer::LauncherServer(AuthServerMgr* authServerMgr):
m_AuthServerMgr(authServerMgr)
{
	m_socketServer.SetAcceptCallback(boost::bind(&LauncherServer::AcceptCallback, this, _1));
}

void LauncherServer::AcceptCallback(Common::network::SocketClient* client)
{
	client->SetCloseCallback(boost::bind(&LauncherServer::CloseCallback, this, _1));
	client->SetReceivePacketCallback(boost::bind(&LauncherServer::PacketCallback, this, _1, _2));
	m_socketClients.insert(client);
}

void LauncherServer::CloseCallback(Common::network::SocketClient* client)
{
	m_socketClients.erase(client);
}

void LauncherServer::PacketCallback(Common::network::SocketClient* client, Common::Buffer_ptr buff)
{
	
}