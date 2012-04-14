#include "SocketClient.h"

#include "SocketServer.h"
#include "../Logger.h"
#include "../crypto/CryptEngine.h"
#include "../stdDefs.h"

#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace Common {
namespace network {

boost::mutex s_IOServiceMutex;
boost::asio::io_service& GetLocalIOService();
void ReleaseLocalIOService();

SocketClient::SocketClient(SocketServer& socketServer):
m_socketServer(&socketServer), m_socket(m_socketServer->GetIOService())
{
	m_thread = boost::thread(boost::bind(&SocketClient::Run, this));
}

SocketClient::SocketClient():
m_socketServer(NULL), m_socket(GetLocalIOService()), m_ReadQueue(10)
{
	m_thread = boost::thread(boost::bind(&SocketClient::Run, this));
}

void SocketClient::Run()
{
	while(!m_thread.interruption_requested()) 
	{
		if(!m_ReadQueue.empty() && m_receiveCallback != NULL)
		{
			m_receiveCallback(m_ReadQueue.front());			
		}
		m_condVarRun.wait(boost::unique_lock<boost::mutex>(boost::mutex()));
	}
}


bool SocketClient::Connect(string& address, string& port)
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::resolver res(m_socket.get_io_service());
    boost::asio::ip::tcp::resolver::query qry(boost::asio::ip::tcp::v4(), address, port);
    boost::asio::ip::tcp::resolver::iterator iteBegin = res.resolve(qry, ec);
    if (ec) {
        return false;
    }

	m_socket.connect(iteBegin->endpoint(), ec);
    if(ec) {
        return false;
    }
	Buffer_ptr readBuff(new Buffer());
	boost::asio::async_read(m_socket,
			boost::asio::buffer(readBuff->Data(), PACKET_HEADER_SIZE),
			boost::bind(&SocketClient::hRead, this,	
				readBuff,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
    return true;
}

void SocketClient::SendPacket(packet::PacketBase& packet)
{
	boost::mutex::scoped_lock lock(m_sendPacketMutex);

	Buffer_ptr buff = packet.GetProcessedBuffer();

	crypto::CryptEngine::GetInstance().XorCrypt(buff, packet.GetKey());

	boost::asio::async_write(m_socket, boost::asio::buffer(buff->Data(), buff->Length()),
		boost::bind(&SocketClient::hWrite, this, buff, boost::asio::placeholders::error));
}

void SocketClient::hWrite(Buffer_ptr buff, const boost::system::error_code& error)
{

}

void SocketClient::hRead(Buffer_ptr buff, const boost::system::error_code& error, size_t size)
{
	if(buff->Get<uint8>(0) == PACKET_START_BIT)
	{
		crypto::CryptEngine::GetInstance().XorDecryptPacketHeader(buff->Data());
		uint16 packetSize = buff->Get<uint16>(2) & ~(PACKET_TYPE_COMPRESSED | PACKET_TYPE_NORMAL);
		
		boost::system::error_code ec;
		boost::asio::read(m_socket,
			boost::asio::buffer(buff->Data() + PACKET_HEADER_SIZE, packetSize - PACKET_HEADER_SIZE),
			ec);
		m_ReadQueue.push_back(buff);

		Buffer_ptr readBuff(new Buffer());
		boost::asio::async_read(m_socket,
			boost::asio::buffer(readBuff->Data(), PACKET_HEADER_SIZE),
			boost::bind(&SocketClient::hRead, this,
				readBuff,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
}

SocketClient::~SocketClient()
{
	if(m_socketServer == NULL)
	{
		ReleaseLocalIOService();
	}
}

int s_IOServiceRefCount = 0;
boost::asio::io_service s_IOService;
boost::asio::io_service::work* s_IOServiceWork = NULL;


boost::asio::io_service& GetLocalIOService()
{
	boost::mutex::scoped_lock(s_IOServiceMutex);
	if(s_IOServiceRefCount == 0)
	{
		s_IOServiceWork = new boost::asio::io_service::work(s_IOService);
        boost::thread(boost::bind(&boost::asio::io_service::run, &s_IOService));
	}
	s_IOServiceRefCount++;
	return s_IOService;
}

void ReleaseLocalIOService()
{
	boost::mutex::scoped_lock(s_IOServiceMutex);
	s_IOServiceRefCount--;
	if(s_IOServiceRefCount == 0)
	{
		delete s_IOServiceWork;
	}
}

}
}