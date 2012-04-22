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
	void AcceptCallback(Common::network::SocketClient* client);
	void CloseCallback(Common::network::SocketClient* client);
	void PacketCallback(Common::network::SocketClient* client, Common::Buffer_ptr buff);
private:
	AuthServerMgr* m_AuthServerMgr;
	Common::network::SocketServer m_socketServer;
	std::set<Common::network::SocketClient*> m_socketClients;
};

#endif //_LAUNCHER_SERVER_H_