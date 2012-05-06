#include "WindowsSocketServer.h"

#include "../../utils.h"
#include "../../typedef.h"

#include "WindowsStartup.h"
#include "WindowsSocketClient.h"

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <string>

namespace Common {
namespace network {
namespace windows {

WindowsSocketServer::WindowsSocketServer(): m_workThreadHandles(new HANDLE[MAX_WORKER_THREAD]), m_lastError(ERR_OK)
{
	if(!WindowsStartup::GetInstance().IsInitialized())
	{
		if(WindowsStartup::GetInstance().HasError() || !WindowsStartup::GetInstance().Initialize())
		{
			m_lastError = ERR_INIT_WINDOWS_SOCKET;
		}
	}

	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if(m_completionPort == NULL)
	{
		m_lastError = ERR_INIT_SOCKETSERVER;
		return;
	}

	m_acceptThreadHandle = CreateThread(NULL, 0,  (LPTHREAD_START_ROUTINE)AcceptThreadProc, this, 0, NULL);
	if(m_acceptThreadHandle == NULL)
	{
		m_lastError = ERR_INIT_SOCKETSERVER;
		return;
	}

	for(int i = 1; i < MAX_WORKER_THREAD; i++)
	{
		HANDLE hThread;
		
		hThread = CreateThread(NULL, 0,  (LPTHREAD_START_ROUTINE)WorkerThreadProc, this, 0, NULL);
		if(hThread == NULL)
		{
			m_lastError = ERR_INIT_SOCKETSERVER;
			return;
		}
		m_workThreadHandles[i] = hThread;
	}

	m_socketListen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED); 
	if( m_socketListen == INVALID_SOCKET )
	{
		m_lastError = ERR_INIT_SOCKETSERVER;
		return;
	}
}

bool WindowsSocketServer::BindAndListen(std::string address, uint16 port)
{
	struct addrinfo hints = {0};
	struct addrinfo *addr = NULL;
	int iRet;

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_IP;

	if(getaddrinfo(address.c_str(), j_itoa(port), &hints, &addr) != 0)
	{
		m_lastError = ERR_RESOLVE_ADDR;
		return false;
	}

	if(addr == NULL)
	{
		m_lastError = ERR_RESOLVE_ADDR;
		return false;
	}

	if(::bind(m_socketListen, addr->ai_addr, (int) addr->ai_addrlen) == SOCKET_ERROR)
	{
		m_lastError = ERR_BIND_SOCKET;
		return false;
	}

	if(listen(m_socketListen, 5) == SOCKET_ERROR )
	{
		m_lastError = ERR_LISTEN_SOCKET;
		return false;
	}

	int zero = 0;
	if(setsockopt(m_socketListen, SOL_SOCKET, SO_SNDBUF, (char* )&zero, sizeof(zero)) == SOCKET_ERROR)
	{
		m_lastError = ERR_SETOPT_SOCKET;
		return false;
	}

	freeaddrinfo(addr);
	return true;
}

WindowsSocketServer::~WindowsSocketServer()
{
	delete[] m_workThreadHandles;
}

DWORD WindowsSocketServer::WorkerThreadProc(WindowsSocketServer* pthis)
{
	return pthis->WorkerThread();
}

DWORD WindowsSocketServer::WorkerThread()
{
	LPWSAOVERLAPPED overlapped = NULL;
	ClientContext* client = NULL;
	BOOL bRet;
	int iRet;
	DWORD flags = 0;
	DWORD numberOfBytes;

	while(true)
	{
		bRet =  GetQueuedCompletionStatus(m_completionPort, &numberOfBytes,
											 (PDWORD_PTR)&client,
											 (LPOVERLAPPED *)&overlapped, 
											 INFINITE);
		if(bRet == FALSE || (bRet == TRUE && numberOfBytes == 0))
		{
			//client desconected
		}

		if(client == NULL)
		{
			return 1;
		}

		/*if(client->Operation == 1)
		{
			client->Operation = 2;
			*client->ReceiveNumBytes() = numberOfBytes;
			client->GetWsaReceiveBuffer()->len = numberOfBytes;
			WSASend(client->GetSocket(), client->GetWsaReceiveBuffer(), 1,
			client->ReceiveNumBytes(), 0, client->GetOverlapperd(), NULL);
		} 
		else
		{
			if(client->GetWsaReceiveBuffer()->len < numberOfBytes)
			{
				client->GetWsaReceiveBuffer()->buf = (char*)client->GetReceiveBuffer()->Data() + numberOfBytes;
				client->GetWsaReceiveBuffer()->len -= numberOfBytes;

				WSASend(client->GetSocket(), client->GetWsaReceiveBuffer(), 1,
					client->ReceiveNumBytes(), 0, client->GetOverlapperd(), NULL);
			}
			else
			{
				client->Operation = 1;
				client->GetReceiveBuffer()->Clear();
				*client->ReceiveNumBytes() = 0;
				client->GetWsaReceiveBuffer()->buf = (char*)client->GetReceiveBuffer()->Data();
				client->GetWsaReceiveBuffer()->len = client->GetReceiveBuffer()->MaxLength();

				iRet = WSARecv(client->GetSocket(), client->GetWsaReceiveBuffer(),
					1, client->ReceiveNumBytes(), &flags, client->GetOverlapperd(), NULL);
				if( iRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
				{
					printf("Error %d", WSAGetLastError());
				}
			}			
		}*/
	}
	return 0;
}

DWORD WindowsSocketServer::AcceptThreadProc(WindowsSocketServer* pthis)
{
	return pthis->AcceptThread();
}

DWORD WindowsSocketServer::AcceptThread()
{
	SOCKET newSocket;
	int iRet;
	DWORD dwRet;
	DWORD flags = 0;

	while(true)
	{
		newSocket = WSAAccept(m_socketListen, NULL, NULL, NULL, 0);
		if(newSocket == SOCKET_ERROR)
		{
			continue;
		}
		
		ClientContext* readClientContext = new ClientContext();
		readClientContext->Client = new WindowsSocketClient(newSocket);

		if(CreateIoCompletionPort((HANDLE)newSocket, m_completionPort, (ULONG_PTR)readClientContext, 0) == NULL)
		{
			delete readClientContext;
			continue;
		}

		iRet = WSARecv(newSocket, client->GetWsaReceiveBuffer(), 1, client->ReceiveNumBytes(), &flags, client->GetOverlapperd(), NULL);
		if( iRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
		{
			printf("Error %d", WSAGetLastError());
		}
	}
}

} //namespace windows
} //namespace network
} //namespace Commom