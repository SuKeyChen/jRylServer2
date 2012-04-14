#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#include "../Buffer.h"
#include "../packet/PacketBase.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/circular_buffer.hpp>

namespace Common {
namespace network {

class SocketServer;

typedef boost::function<void (Buffer_ptr)> PacketCallback;

class SocketClient
{
public:
	SocketClient();
	SocketClient(SocketServer& socketServer);

	~SocketClient();

	bool Connect(string& address, string& port);
	void SendPacket(packet::PacketBase& packet);
	
	inline void ReceivePacketCallback(PacketCallback packetCallback);
private:
	void Run();
	void hWrite(Buffer_ptr buff, const boost::system::error_code& error);
	void hRead(Buffer_ptr buff, const boost::system::error_code& error, size_t size);
private:
	boost::condition_variable m_condVarRun;
	boost::asio::ip::tcp::socket m_socket;
	PacketCallback m_receiveCallback;
	SocketServer* m_socketServer;
	boost::thread m_thread;
	boost::mutex m_sendPacketMutex;
	boost::circular_buffer<Buffer_ptr> m_ReadQueue;
};

void SocketClient::ReceivePacketCallback(PacketCallback receiveCallback)
{
	m_receiveCallback = receiveCallback;
}

}
}

#endif