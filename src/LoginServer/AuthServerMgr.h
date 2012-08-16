#ifndef _AUTHSERVERMGR_H_
#define _AUTHSERVERMGR_H_

#include "../Common/stdDefs.h"
#include "../Common/network/SocketServer.h"
#include "../Common/network/SocketClient.h"

#include <map>

class AuthServer;
struct UnauthorizedAuthServer {
    enum UnauthorizedAuthServerState {
        US_WAIT_PING,
        US_WAIT_USERPASS
    };
    UnauthorizedAuthServerState State;
    time_t Time;
    int32 AttemptLogin;
};

class AuthServerMgr
{
public:
	AuthServerMgr();
	~AuthServerMgr();
	
	bool Start();
	void RemoveAuthServer(AuthServer* authServer);

	inline AuthServer** GetAuthServers();
private:
	void EventCallbakc(Common::network::SocketServerBase* server, Common::network::ServerEventType type, void* arg);
	void ClientConnect(Common::network::SocketClientBase* client);
	void ClientDesconnect(Common::network::SocketClientBase* client);	
	
	void EventCallbakcUnauthorized(Common::network::SocketClientBase* client, Common::network::ClientEventType type, void* arg);
	void PacketParserUnauthorized(Common::network::SocketClientBase* client, Common::Buffer_ptr buff);
private:
	AuthServer* m_authServers[AUTHSERVER_SLOT_LENGTH];
	std::map<Common::network::SocketClientBase*, UnauthorizedAuthServer> m_unauthorizedAuthServers;
	Common::network::SocketServerBase* m_socketListen;
};

AuthServer** AuthServerMgr::GetAuthServers()
{
	return m_authServers;
}

#endif