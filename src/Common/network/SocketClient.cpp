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

SocketClient::SocketClient(boost::asio::ip::tcp::socket* socket, SocketServer& socketServer):
m_socketServer(&socketServer), m_socket(socket), m_readQueue(10), m_isClosing(false)
{
	m_thread = boost::thread(boost::bind(&SocketClient::Run, this));

	Buffer_ptr readBuff(new Buffer());
	boost::asio::async_read(*m_socket,
			boost::asio::buffer(readBuff->Data(), PACKET_HEADER_SIZE),
			boost::bind(&SocketClient::hRead, this,	
				readBuff,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
}

SocketClient::SocketClient():
m_socketServer(NULL), m_socket(new boost::asio::ip::tcp::socket(GetLocalIOService())),
m_readQueue(10), m_isClosing(false)
{
	m_thread = boost::thread(boost::bind(&SocketClient::Run, this));
}

void SocketClient::Run()
{
	while(!m_thread.interruption_requested()) 
	{
		while(!m_readQueue.empty() && m_receiveCallback != NULL)
		{
			m_readQueueMutex.lock();
			Buffer_ptr buff = m_readQueue[0];
			m_readQueue.pop_front();
			m_readQueueMutex.unlock();
			m_receiveCallback(this, buff);			
		}
		m_condVarRun.timed_wait(boost::unique_lock<boost::mutex>(boost::mutex()), boost::posix_time::seconds(1));
	}
}


bool SocketClient::Connect(string address, string port)
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::resolver res(m_socket->get_io_service());
    boost::asio::ip::tcp::resolver::query qry(boost::asio::ip::tcp::v4(), address, port);
    boost::asio::ip::tcp::resolver::iterator iteBegin = res.resolve(qry, ec);
    if (ec)
	{
        return false;
    }

	m_socket->connect(iteBegin->endpoint(), ec);
    if(ec)
	{
        return false;
    }
	Buffer_ptr readBuff(new Buffer());
	boost::asio::async_read(*m_socket,
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

	boost::asio::async_write(*m_socket, boost::asio::buffer(buff->Data(), buff->Length()),
		boost::bind(&SocketClient::hWrite, this, buff, boost::asio::placeholders::error));
}

void SocketClient::hWrite(Buffer_ptr buff, const boost::system::error_code& error)
{
	if(error) 
	{
		Close();
	}
}

void SocketClient::hRead(Buffer_ptr buff, const boost::system::error_code& error, size_t size)
{
	if(error) 
	{
		Close();
	}

	buff->SetLength(size);
	if(buff->Get<uint8>(0) == PACKET_START_BIT)
	{
		crypto::CryptEngine::GetInstance().XorDecryptPacketHeader(buff->Data());
		uint16 packetSize = buff->Get<uint16>(2) & ~(PACKET_TYPE_COMPRESSED | PACKET_TYPE_NORMAL);
		
		boost::system::error_code ec;
		boost::asio::read(*m_socket,
			boost::asio::buffer(buff->Data() + PACKET_HEADER_SIZE, packetSize - PACKET_HEADER_SIZE),
			ec);
		buff->SetLength(packetSize);
		Common::crypto::CryptEngine::Cryptkey key;
		buff->GetPack(key, 4);
		crypto::CryptEngine::GetInstance().XorDecryptPacketBody(buff->Data(), buff->Length(), key);		

		m_readQueueMutex.lock();
		m_readQueue.push_back(buff);
		m_readQueueMutex.unlock();

		m_condVarRun.notify_all();

		Buffer_ptr readBuff(new Buffer());
		boost::asio::async_read(*m_socket,
			boost::asio::buffer(readBuff->Data(), PACKET_HEADER_SIZE),
			boost::bind(&SocketClient::hRead, this,
				readBuff,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
}

void SocketClient::Close()
{
	boost::mutex::scoped_lock(m_closingMutex);
	if(!m_isClosing)
	{
		m_isClosing = true;
	}
	else
	{
		return;
	}
	boost::system::error_code ec;
	m_socket->close(ec);
	if(m_closeCallback != NULL)
	{
		m_closeCallback(this);
	}
}

SocketClient::~SocketClient()
{
	Close();
	if(m_socketServer == NULL)
	{
		ReleaseLocalIOService();
	}
	if(m_socket != NULL)
	{
		delete m_socket;
	}
}

int s_IOServiceRefCount = 0;
boost::asio::io_service* s_IOService;
boost::asio::io_service::work* s_IOServiceWork = NULL;

boost::asio::io_service& GetLocalIOService()
{
	boost::mutex::scoped_lock(s_IOServiceMutex);
	if(s_IOServiceRefCount == 0)
	{
		s_IOService = new boost::asio::io_service();
		s_IOServiceWork = new boost::asio::io_service::work(*s_IOService);
        boost::thread(boost::bind(&boost::asio::io_service::run, s_IOService));
	}
	s_IOServiceRefCount++;
	return *s_IOService;
}

void ReleaseLocalIOService()
{
	boost::mutex::scoped_lock(s_IOServiceMutex);
	s_IOServiceRefCount--;
	if(s_IOServiceRefCount == 0)
	{
		delete s_IOService;
		delete s_IOServiceWork;
	}
}

}
}