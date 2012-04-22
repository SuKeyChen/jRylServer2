#include "SocketServer.h"

#include "SocketClient.h"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

namespace Common {
namespace network {

SocketServer::SocketServer():
m_work(m_ioservice), m_acceptor(m_ioservice),
m_acceptCallback(NULL)
{
	boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioservice));
}

bool SocketServer::BindAndListen(std::string address, std::string port)
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::resolver res(m_ioservice);
	boost::asio::ip::tcp::resolver::query qry(boost::asio::ip::tcp::v4(), std::string(address), port);
    boost::asio::ip::tcp::resolver::iterator iteBegin = res.resolve(qry, ec);
    if (ec)
	{        
		return false;
	}
    boost::asio::ip::tcp::resolver::iterator iteEnd;
	m_acceptor.open(iteBegin->endpoint().protocol(), ec);
    if (ec)
	{
       return false;
    }
	while (iteBegin != iteEnd)
	{
        m_acceptor.bind(iteBegin->endpoint(), ec);
        if (ec)
		{
            return false;
        }
        iteBegin++;
    }

	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
	m_acceptor.listen(100, ec);
    if (ec) {
        return false;
    }


	boost::asio::ip::tcp::socket* socket = new boost::asio::ip::tcp::socket(m_ioservice);
	m_acceptor.async_accept(*socket,
      boost::bind(&SocketServer::hAccept, this, socket,boost::asio::placeholders::error));

	return true;
}

void SocketServer::hAccept(boost::asio::ip::tcp::socket* socket, const boost::system::error_code& ec)
{
	if(ec)
	{
	}
	else
	{
		if(m_acceptCallback != NULL)
		{
			SocketClient* client = new SocketClient(socket, *this);
			m_acceptCallback(client);
			
			socket = new boost::asio::ip::tcp::socket(m_ioservice);
		}		
		m_acceptor.async_accept(*socket,
			boost::bind(&SocketServer::hAccept, this, socket,boost::asio::placeholders::error));
	}
}

SocketServer::~SocketServer()
{

}

}
}