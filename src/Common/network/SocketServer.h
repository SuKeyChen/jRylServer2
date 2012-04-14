#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

namespace Common {
namespace network {

#include <boost/asio/io_service.hpp>

class SocketServer
{
public:
	SocketServer();
	~SocketServer();
	//inline boost::asio::io_service& GetIOService();
private:
	//)boost::asio::io_service m_ioservice;
	//boost::asio::io_service::work m_work;
};

//boost::asio::io_service& SocketServer::GetIOService()
//{
//	return m_ioservice;
//}

}
}

#endif