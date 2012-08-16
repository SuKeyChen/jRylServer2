#ifndef _LAUNCHER_SERVER_H_
#define _LAUNCHER_SERVER_H_

#include "../Common/network/SocketServer.h"
#include "../Common/network/SocketClient.h"
#include "../Common/Buffer.h"

#include <set>

class AuthServerMgr;

class LauncherServer
{
public:
	LauncherServer(AuthServerMgr* authServerMgr);
private:
	void ServerEventCallback(Common::network::SocketServerBase* server, Common::network::ServerEventType type, void* arg);
	void ClientEventCallback(Common::network::SocketClientBase* client, Common::network::ClientEventType type, void* arg);
	void AcceptCallback(Common::network::SocketClientBase* client);
	void CloseCallback(Common::network::SocketClientBase* client);
	void PacketCallback(Common::network::SocketClientBase* client, Common::Buffer_ptr buff);
private:
	AuthServerMgr* m_authServerMgr;
	Common::network::SocketServerBase* m_socketServer;
	std::set<Common::network::SocketClientBase*> m_socketClients;
};

#endif //_LAUNCHER_SERVER_H_