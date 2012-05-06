#include "WindowsSocketClient.h"

#include "../../Buffer.h"

#include <WinSock2.h>
#include <Windows.h>
#include <string>

namespace Common {
namespace network {
namespace windows {

WindowsSocketClient::WindowsSocketClient(SOCKET socket)
{
	m_socket = socket;

	memset(&m_overlapperd, 0, sizeof(WSAOVERLAPPED));
	m_overlapperd.hEvent = NULL;
}

} //namespace windows
} //namespace network
} //namespace Commom