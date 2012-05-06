#include "SocketServer.h"

#include "SocketClient.h"

namespace Common {
namespace network {

SocketServer::SocketServer():
m_acceptCallback(NULL)
{
	
}

bool SocketServer::BindAndListen(std::string address, std::string port)
{
	return true;
}

SocketServer::~SocketServer()
{
}

}
}