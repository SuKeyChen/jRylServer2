#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
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
	inline boost::asio::io_service& GetIOService();
	inline void SetAcceptCallback(ClientCallback acceptCallback);
private:
	void hAccept(boost::asio::ip::tcp::socket* socket, const boost::system::error_code& ec);
private:
	ClientCallback m_acceptCallback;
	boost::asio::io_service m_ioservice;
	boost::asio::io_service::work m_work;
	boost::asio::ip::tcp::acceptor m_acceptor;
};

boost::asio::io_service& SocketServer::GetIOService()
{
	return m_ioservice;
}

void SocketServer::SetAcceptCallback(ClientCallback acceptCallback)
{
	m_acceptCallback = acceptCallback;
}

}
}

#endif