#ifndef _WINDOWS_SOCKETSERVER_H_
#define _WINDOWS_SOCKETSERVER_H_

#include "WindowsCommonDefs.h"
#include "../../typedef.h"
#include "../SocketServerBase.h"

#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <list>

namespace Common {
namespace network {
namespace windows {

class WindowsSocketClient;

class WindowsSocketServer: public SocketServerBase
{
public:
	WindowsSocketServer();
	~WindowsSocketServer();

	virtual bool BindAndListen(std::string address, uint16 port);
	void NotifyAvaliableBytesToSend(WindowsSocketClient* client);
private:
	ClientContext* GetClientSendContext(WindowsSocketClient* client);
	ClientContext* GetClientContextByOverlapped(WSAOVERLAPPED* overlapped);
	void ClearClient(WindowsSocketClient* client);

	static DWORD WorkerThreadProc(WindowsSocketServer* pthis);
	DWORD WorkerThread();
	static DWORD AcceptThreadProc(WindowsSocketServer* pthis);
	DWORD AcceptThread();
private:
	HANDLE m_completionPort;
	SOCKET m_socketListen;
	HANDLE* m_workThreadHandles;
	HANDLE m_acceptThreadHandle;
	std::list<ClientContext*> m_allContexts;
};

} //namespace windows
} //namespace network
} //namespace Commom

#endif //_WINDOWS_SOCKETSERVER_H_
