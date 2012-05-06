#ifndef _WINDOWS_SOCKETSERVER_H_
#define _WINDOWS_SOCKETSERVER_H_

#include "../../typedef.h"

#include <WinSock2.h>
#include <Windows.h>
#include <string>

namespace Common {
namespace network {
namespace windows {

class WindowsSocketClient;

#define MAX_WORKER_THREAD 16

typedef void (*ClientCallback)(WindowsSocketClient*);

class WindowsSocketServer
{
public:
	enum Errors
	{
		ERR_OK, ERR_INIT_WINDOWS_SOCKET, ERR_INIT_SOCKETSERVER, ERR_RESOLVE_ADDR, ERR_BIND_SOCKET,
		ERR_LISTEN_SOCKET, ERR_SETOPT_SOCKET
	};
private:
	enum ClientContextType 
	{
		CCT_RECEIVE, CCT_SEND
	};
	struct ClientContext
	{
		WindowsSocketClient* Client;
		WSAOVERLAPPED Overlapped;
		WSABUF Buffer;		
		ClientContextType Type;
		DWORD NumBytes;
	};
public:
	WindowsSocketServer();
	~WindowsSocketServer();

	bool BindAndListen(std::string address, uint16 port);
	inline void SetAcceptCallback(ClientCallback acceptCallback);
	inline Errors GetLastError();
private:
	static DWORD WorkerThreadProc(WindowsSocketServer* pthis);
	DWORD WorkerThread();
	static DWORD AcceptThreadProc(WindowsSocketServer* pthis);
	DWORD AcceptThread();
private:
	ClientCallback m_acceptCallback;
	HANDLE m_completionPort;
	SOCKET m_socketListen;
	HANDLE* m_workThreadHandles;
	HANDLE m_acceptThreadHandle;
	Errors m_lastError;
};

void WindowsSocketServer::SetAcceptCallback(ClientCallback acceptCallback)
{
	m_acceptCallback = acceptCallback;
}

WindowsSocketServer::Errors WindowsSocketServer::GetLastError()
{
	return m_lastError;
}

} //namespace windows
} //namespace network
} //namespace Commom

#endif //_WINDOWS_SOCKETSERVER_H_
