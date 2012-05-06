#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

#include <boost/function.hpp>

namespace Common {
namespace network {

class SocketClient;

typedef boost::function<void (SocketClient*)> ClientCallback;

class SocketServer
{
public:
	SocketServer();	
	~SocketServer();

	bool BindAndListen(std::string address, std::string port);
	inline void SetAcceptCallback(ClientCallback acceptCallback);
private:
	ClientCallback m_acceptCallback;
};

void SocketServer::SetAcceptCallback(ClientCallback acceptCallback)
{
	m_acceptCallback = acceptCallback;
}

}
}

#endif