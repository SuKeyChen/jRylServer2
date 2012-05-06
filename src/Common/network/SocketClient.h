#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#include "../Buffer.h"
#include "../packet/PacketBase.h"

#include <boost/function.hpp>
#include <boost/circular_buffer.hpp>

#include <queue>

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
	~SocketClient();

	bool Connect(string address, string port);
	void SendPacket(packet::PacketBase& packet);
	void Close();
	
	inline void SetReceivePacketCallback(PacketCallback packetCallback);
	inline void SetCloseCallback(ClientCallback closeCallback);
private:
	PacketCallback m_receiveCallback;
	ClientCallback m_closeCallback;
	SocketServer* m_socketServer;
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