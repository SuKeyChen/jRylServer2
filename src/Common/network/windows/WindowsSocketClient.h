#ifndef WINDOWS_SOCKETCLIENT_H_
#define WINDOWS_SOCKETCLIENT_H_

#include "../../packet/PacketBase.h"
#include "../../Buffer.h"
#include "../SocketClientBase.h"
#include "WindowsCommonDefs.h"

#include <boost/function.hpp>

#include <WinSock2.h>
#include <Windows.h>
#include <queue>

namespace Common {
namespace network {
namespace windows {

class WindowsSocketClient;

class WindowsSocketClient: public SocketClientBase
{
friend class WindowsSocketServer;
public:
	WindowsSocketClient();
	WindowsSocketClient(SOCKET socket, WindowsSocketServer* server);

	virtual bool Connect(string address, string port);
	virtual void Desconect();
	virtual void SendPacket(packet::PacketBase& packet);
	void SendBytes(byte* bytes, size_t size);
	void AddBytesToReveivedQueue(byte* bytes, size_t size);
	size_t GetBytesFromSendQueue(byte* bytes, size_t maxSize);
	inline SOCKET GetSocket();
private:
	void ProcessSendQueue();

	static DWORD WorkerThreadProc(WindowsSocketClient* pthis);
	DWORD WorkerThread();
private:
	SOCKET m_socket;
	WindowsSocketServer* m_server;
	HANDLE m_completionPort;
	HANDLE m_thread;
	ClientContext* m_readClientContext;
	ClientContext* m_writeClientContext;
};

SOCKET WindowsSocketClient::GetSocket()
{
	return m_socket;
}

} //namespace windows
} //namespace network
} //namespace Commom

#endif //WINDOWS_SOCKETCLIENT_H_