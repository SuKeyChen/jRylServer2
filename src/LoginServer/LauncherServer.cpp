#include "LauncherServer.h"


#include "AuthServer.h"
#include "AuthServerMgr.h"
#include "GlobalConfTable.h"

#include "../Common/packet/pktPing.h"
#include "../Common/packet/pktServerList.h"
#include "../Common/packet/pktServerListAsk.h"

#include <boost/bind.hpp>

using namespace Common;

LauncherServer::LauncherServer(AuthServerMgr* authServerMgr):
	m_authServerMgr(authServerMgr), m_socketServer(new Common::network::SocketServer())
{
	m_socketServer->SetServerCallback(boost::bind(&LauncherServer::ServerEventCallback, this, _1, _2, _3));
}

void LauncherServer::AcceptCallback(Common::network::SocketClientBase* client)
{
	client->SetEventCallback(boost::bind(&LauncherServer::ClientEventCallback, this, _1, _2, _3));
	m_socketClients.insert(client);
}

void LauncherServer::CloseCallback(Common::network::SocketClientBase* client)
{
	m_socketClients.erase(client);
}

void LauncherServer::PacketCallback(Common::network::SocketClientBase* client, Common::Buffer_ptr buff)
{
	GlobalConfTable* gconf = GlobalConfTable::GetInstance_ptr();

	buff->SetReaderOffset(0);
	
	switch(buff->Get<uint8>(1)) {
	case packet::Ping::PacketID:
		break;
	case packet::ServerListAsk::PacketID:
		packet::ServerList sl;
		AuthServer** authServerList = m_authServerMgr->GetAuthServers();
		packet::ServerList::AuthServer* authserver = sl.GetAuthServers();
		int numServer = 0;
		for(int i = 0; i < AUTHSERVER_SLOT_LENGTH; i++) {
			if(authServerList[i] != NULL && authServerList[i]->GetStatus() == AuthServer::ASS_OK) {
				authserver[numServer].Group = authServerList[i]->GetGroup();
				authserver[numServer].Name = authServerList[i]->GetName();
				authserver[numServer].Address = authServerList[i]->GetAddress();
				authserver[numServer].ClientNum[0] = 0;
				authserver[numServer].ClientNum[1] = 0;
				numServer++;
			}
		}
		if(numServer > 0) {		
			sl.SetClientVersion(gconf->PatchVersion);
			sl.SetPatchAddress(gconf->PatchAddress);
			sl.SetNumServers(numServer);
		} else {
			sl.SetErro(packet::ServerList::SLE_ServerUnavailable);
		}
		client->SendPacket(sl);
		break;
	}
}

void LauncherServer::ServerEventCallback(Common::network::SocketServerBase* server, Common::network::ServerEventType type, void* arg)
{
	switch (type)
	{
	case Common::network::SEVT_CLIENT_CONNECTED:
		AcceptCallback((Common::network::SocketClientBase*)arg);
		break;
	case Common::network::SEVT_CLIENT_DESCONNECTED:
		CloseCallback((Common::network::SocketClientBase*)arg);
		break;
	default:
		break;
	}
}

void LauncherServer::ClientEventCallback(Common::network::SocketClientBase* client, Common::network::ClientEventType type, void* arg)
{
	switch (type)
	{
	case Common::network::CEV_PACKET_RECEIVED:
		PacketCallback(client, *(Common::Buffer_ptr*)arg);
		break;
	case Common::network::CEV_CLIENT_DESCONNECTED:
		CloseCallback(client);
		break;
	default:
		break;
	}
}