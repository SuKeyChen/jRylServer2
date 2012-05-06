#ifndef _AUTHSERVER_H_
#define _AUTHSERVER_H_

#include "../Common/network/SocketClient.h"
#include "../Common/typedef.h"

class AuthServerMgr;

class AuthServer
{
public:
	enum AuthServerStatus {
		ASS_OK,
		ASS_WAIT_DETAILS
	};
public:
	AuthServer(Common::network::SocketClient* client, AuthServerMgr* authServerMgr);
	~AuthServer();

	inline uint8 GetGroup();
    inline string GetName();
    inline uint32 GetAddress();
    inline uint32 GetLastPing();
	inline AuthServerStatus GetStatus();
private:
	void PacketParser(Common::network::SocketClient* client, Common::Buffer_ptr buff);
	void ClientDesconnect(Common::network::SocketClient* client);
private:
	uint8 m_group;
    string m_name;
    uint32 m_address;
    uint32 m_lastPing;
    Common::network::SocketClient* m_socketClient;
	AuthServerStatus m_status;
	AuthServerMgr* m_authServerMgr;
};

inline uint8 AuthServer::GetGroup() {
	return m_group;
}

inline string AuthServer::GetName() {
	return m_name;
}

inline uint32 AuthServer::GetAddress() {
	return m_address;
}

inline uint32 AuthServer::GetLastPing() {
	return m_lastPing;
}

inline AuthServer::AuthServerStatus AuthServer::GetStatus() {
	return m_status;
}

#endif