#ifndef WINDOWS_SOCKETCLIENT_H_
#define WINDOWS_SOCKETCLIENT_H_

#include <WinSock2.h>
#include <Windows.h>
#include <string>

#define READ_BUFFER_SIZE

namespace Common {
namespace network {
namespace windows {

class WindowsSocketClient
{
public:
	WindowsSocketClient(SOCKET socket);
	inline WSABUF* GetWsaReceiveBuffer();
	inline WSAOVERLAPPED* GetOverlapperd();
	inline DWORD* ReceiveNumBytes();
	inline SOCKET GetSocket();
private:
	SOCKET m_socket;
};

SOCKET WindowsSocketClient::GetSocket()
{
	return m_socket;
}

//Buffer_ptr WindowsSocketClient::GetReceiveBuffer()
//{
//	return m_receiveBuffer;
//}

} //namespace windows
} //namespace network
} //namespace Commom

#endif //WINDOWS_SOCKETCLIENT_H_