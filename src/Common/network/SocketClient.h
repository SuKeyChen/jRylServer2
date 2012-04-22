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
class SocketClient;

typedef boost::function<void (SocketClient*, Buffer_ptr)> PacketCallback;
typedef boost::function<void (SocketClient*)> ClientCallback;

class SocketClient
{
public:
	SocketClient();
	SocketClient(boost::asio::ip::tcp::socket* socket, SocketServer& socketServer);

	~SocketClient();

	bool Connect(string address, string port);
	void SendPacket(packet::PacketBase& packet);
	void Close();
	
	inline void SetReceivePacketCallback(PacketCallback packetCallback);
	inline void SetCloseCallback(ClientCallback closeCallback);
private:
	void Run();
	void hWrite(Buffer_ptr buff, const boost::system::error_code& error);
	void hRead(Buffer_ptr buff, const boost::system::error_code& error, size_t size);
private:
	bool m_isClosing;
	boost::condition_variable m_condVarRun;
	boost::asio::ip::tcp::socket* m_socket;
	PacketCallback m_receiveCallback;
	ClientCallback m_closeCallback;
	SocketServer* m_socketServer;
	boost::thread m_thread;
	boost::mutex m_closingMutex;
	boost::mutex m_sendPacketMutex;
	boost::mutex m_readQueueMutex;
	boost::circular_buffer<Buffer_ptr> m_readQueue;
};

void SocketClient::SetReceivePacketCallback(PacketCallback receiveCallback)
{
	m_receiveCallback = receiveCallback;
}

void SocketClient::SetCloseCallback(ClientCallback closeCallback)
{
	m_closeCallback = closeCallback;
}

}
}

#endif