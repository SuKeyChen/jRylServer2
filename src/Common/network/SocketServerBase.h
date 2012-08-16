#ifndef _SOCKET_SERVER_BASE_H_
#define _SOCKET_SERVER_BASE_H_

#include <boost/function.hpp>

#define SOCKETSERVER_IMP Common::network::windows::WindowsSocketServer

namespace Common {
namespace network {

class SocketClientBase;
class SocketServerBase;

enum Errors
{
	ERR_OK, ERR_INIT_WINDOWS_SOCKET, ERR_INIT_SOCKETSERVER, ERR_RESOLVE_ADDR, ERR_BIND_SOCKET,
	ERR_LISTEN_SOCKET, ERR_SETOPT_SOCKET
};

enum ServerEventType
{
	SEVT_CLIENT_CONNECTED, SEVT_CLIENT_DESCONNECTED
};

typedef boost::function<void (SocketServerBase*, ServerEventType, void*)> ServerEventCallback;

class SocketServerBase
{
public:
	virtual bool BindAndListen(std::string address, uint16 port) = 0;
	inline void SetServerCallback(ServerEventCallback callback);
	inline Errors GetLastError();
protected:
	ServerEventCallback m_callback;
	Errors m_lastError;
};

void SocketServerBase::SetServerCallback(ServerEventCallback callback)
{
	m_callback = callback;
}

Errors SocketServerBase::GetLastError()
{
	return m_lastError;
}

}
}

#endif