#ifndef _WINDOWS_COMMON_DEFS_
#define _WINDOWS_COMMON_DEFS_

#include <WinSock2.h>

namespace Common {
namespace network {
namespace windows {

#define MAX_WORKER_THREAD 16
#define TCP_BUFFER_SIZE 1024

class WindowsSocketClient;

enum ClientContextState
{
	CCT_RECEIVE, CCT_SEND, CCT_SEND_NOT_WORK
};
struct ClientContext
{
	WindowsSocketClient* Client;
	WSAOVERLAPPED Overlapped;
	WSABUF WSBuffer;
	byte Buffer[TCP_BUFFER_SIZE];
	ClientContextState State;
	DWORD NumBytes;
};

}
}
}

#endif