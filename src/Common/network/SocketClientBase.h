#ifndef _SOCKET_CLIENT_BASE_H_
#define _SOCKET_CLIENT_BASE_H_

#include "../Buffer.h"
#include "../packet/PacketBase.h"

#include <boost/function.hpp>
#include <boost/circular_buffer.hpp>

#include <queue>

namespace Common {
namespace network {

enum ClientEventType
{
	CEV_PACKET_RECEIVED,
	CEV_CLIENT_DESCONNECTED
};

class SocketServerBase;
class SocketClientBase;

typedef boost::function<void (SocketClientBase*, ClientEventType , void*)> ClientEventCallback;

class SocketClientBase
{
public:
	SocketClientBase();
	virtual bool Connect(string address, string port) = 0;
	virtual void SendPacket(packet::PacketBase& packet) = 0;
	virtual void Desconect() = 0;
	
	inline void SetEventCallback(ClientEventCallback closeCallback);
protected:
	void ProcessReceivedBytes();
protected:
	ClientEventCallback m_eventCallback;
	std::queue<byte> m_receiveQueue;
	std::queue<byte> m_sendQueue;
private:
	Buffer_ptr m_processingBuffer;
};

void SocketClientBase::SetEventCallback(ClientEventCallback callback)
{
	m_eventCallback = callback;
}

}
}

#endif