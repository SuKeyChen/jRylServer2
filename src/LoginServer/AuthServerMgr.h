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
	void ClientConnect(Common::network::SocketClient* client);
	void PacketParserUnauthorized(Common::network::SocketClient* client, Common::Buffer_ptr buff);
	void ClientDesconnect(Common::network::SocketClient* client);
private:
	AuthServer* m_authServers[AUTHSERVER_SLOT_LENGTH];
	std::map<Common::network::SocketClient*, UnauthorizedAuthServer> m_unauthorizedAuthServers;
	Common::network::SocketServer m_socketListen;	
};

AuthServer** AuthServerMgr::GetAuthServers()
{
	return m_authServers;
}

#endif