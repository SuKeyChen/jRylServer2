#include "WindowsSocketServer.h"

#include "../../utils.h"
#include "../../typedef.h"

#include "WindowsStartup.h"
#include "WindowsSocketClient.h"
#include "WindowsCommonDefs.h"

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <string>

namespace Common {
namespace network {
namespace windows {

WindowsSocketServer::WindowsSocketServer():
	m_workThreadHandles(new HANDLE[MAX_WORKER_THREAD])
{
	m_lastError = ERR_OK;
	if(!WindowsStartup::GetInstance().IsInitialized())
	{
		if(!WindowsStartup::GetInstance().Initialize())
		{
			m_lastError = ERR_INIT_WINDOWS_SOCKET;
			return;
		}
	}

	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if(m_completionPort == NULL)
	{
		m_lastError = ERR_INIT_SOCKETSERVER;
		return;
	}

	m_acceptThreadHandle = CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)AcceptThreadProc, this, CREATE_SUSPENDED, NULL);
	if(m_acceptThreadHandle == NULL)
	{
		m_lastError = ERR_INIT_SOCKETSERVER;
		return;
	}

	for(int i = 1; i < MAX_WORKER_THREAD; i++)
	{
		HANDLE hThread;
		
		hThread = CreateThread(NULL, 0,  (LPTHREAD_START_ROUTINE)WorkerThreadProc,
			this, 0, NULL);
		if(hThread == NULL)
		{
			m_lastError = ERR_INIT_SOCKETSERVER;
			return;
		}
		m_workThreadHandles[i] = hThread;
	}

	m_socketListen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0,
		WSA_FLAG_OVERLAPPED); 
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

	ResumeThread(m_acceptThreadHandle);

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
	WindowsSocketClient* client = NULL;
	BOOL bRet;
	int iRet;
	DWORD flags = 0;
	DWORD numberOfBytes;

	while(true)
	{
		bRet =  GetQueuedCompletionStatus(m_completionPort, &numberOfBytes,
											 (PDWORD_PTR)&client,
											 (LPOVERLAPPED*)&overlapped, 
											 INFINITE);
		
		

		if(bRet == FALSE || (bRet == TRUE && numberOfBytes == 0))
		{
			ClearClient(client);
			
			continue;
		}

		if(client == NULL)
		{
			continue;
		}		

		ClientContext* clientContext = GetClientContextByOverlapped(overlapped);

		if(clientContext->State == CCT_RECEIVE)
		{
			clientContext->Client->AddBytesToReveivedQueue(clientContext->Buffer, numberOfBytes);
			
			iRet = WSARecv(clientContext->Client->GetSocket(), &clientContext->WSBuffer,
				1, &clientContext->NumBytes, &flags, &clientContext->Overlapped, NULL);
			if( iRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
			{
				ClearClient(client);
			}
		}
		else
		{
			if(clientContext->NumBytes > numberOfBytes)
			{
				clientContext->WSBuffer.buf += numberOfBytes;
				clientContext->WSBuffer.len -= numberOfBytes;
				clientContext->NumBytes -= numberOfBytes;

				iRet = WSASend(clientContext->Client->GetSocket(), &clientContext->WSBuffer,
					1, &clientContext->NumBytes, flags, &clientContext->Overlapped, NULL);
			}
			else
			{
				size_t size = clientContext->Client->GetBytesFromSendQueue(clientContext->Buffer, TCP_BUFFER_SIZE);
				if(size > 0)
				{
					clientContext->WSBuffer.buf = (char*)clientContext->Buffer;
					clientContext->WSBuffer.len = size;
					clientContext->NumBytes = size;

					iRet = WSASend(clientContext->Client->GetSocket(), &clientContext->WSBuffer,
						1, &clientContext->NumBytes, flags, &clientContext->Overlapped, NULL);					
				}
				else				
				{
					clientContext->State = CCT_SEND_NOT_WORK;
				}
			}
		}
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
	DWORD flags = 0;

	while(true)
	{
		newSocket = WSAAccept(m_socketListen, NULL, NULL, NULL, 0);
		
		if(newSocket == SOCKET_ERROR)
		{
			continue;
		}
		
		ClientContext* readClientContext = new ClientContext();
		ClientContext* writeClientContext = new ClientContext();
		WindowsSocketClient* client = new WindowsSocketClient(newSocket, this);

		if(CreateIoCompletionPort((HANDLE)newSocket, m_completionPort,
			(ULONG_PTR)client, 0) == NULL)
		{
			delete readClientContext, writeClientContext, client;
			continue;
		}

		readClientContext->State = CCT_RECEIVE;
		readClientContext->Client = client;
		readClientContext->WSBuffer.buf = (char*)readClientContext->Buffer;
		readClientContext->WSBuffer.len = TCP_BUFFER_SIZE;

		writeClientContext->State = CCT_SEND_NOT_WORK;
		writeClientContext->Client = client;

		m_allContexts.push_back(readClientContext);
		m_allContexts.push_back(writeClientContext);

		iRet = WSARecv(newSocket, &readClientContext->WSBuffer, 1, &readClientContext->NumBytes,
			&flags, &readClientContext->Overlapped, NULL);
		if( iRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
		{
			closesocket(newSocket);
			delete readClientContext, writeClientContext, client;
			continue;
		}
		m_callback(this, SEVT_CLIENT_CONNECTED, client);
	}
}

void WindowsSocketServer::NotifyAvaliableBytesToSend(WindowsSocketClient* client)
{
	ClientContext* clientContext = GetClientSendContext(client);
	size_t size = clientContext->Client->GetBytesFromSendQueue(clientContext->Buffer, TCP_BUFFER_SIZE);
	if(size > 0)
	{
		clientContext->WSBuffer.buf = (char*)clientContext->Buffer;
		clientContext->WSBuffer.len = size;
		clientContext->NumBytes = size;
		int iRet;
		WSASend(clientContext->Client->GetSocket(), &clientContext->WSBuffer,
			1, &clientContext->NumBytes, 0, &clientContext->Overlapped, NULL);

		clientContext->State = CCT_SEND;
	}
}

ClientContext* WindowsSocketServer::GetClientSendContext(WindowsSocketClient* client)
{
	list<ClientContext*>::iterator it =  m_allContexts.begin();
	
	while(it != m_allContexts.end())
	{
		if((*it)->Client == client)
		{
			return *it;
		}
		it++;
	}
	return NULL;
}

ClientContext* WindowsSocketServer::GetClientContextByOverlapped(WSAOVERLAPPED* overlapped)
{
	list<ClientContext*>::iterator it =  m_allContexts.begin();
	
	while(it != m_allContexts.end())
	{
		if(&(*it)->Overlapped == overlapped)
		{
			return *it;
		}
		it++;
	}
	return NULL;
}

void WindowsSocketServer::ClearClient(WindowsSocketClient* client)
{
	assert(client != NULL);

	closesocket(client->GetSocket());

	if(m_callback != NULL)
	{
		m_callback(this, SEVT_CLIENT_DESCONNECTED, client);			
	}
	
	list<ClientContext*>::iterator it =  m_allContexts.begin();
	ClientContext* context1 = NULL;
	ClientContext* context2 = NULL;

	while(it != m_allContexts.end())
	{
		if((*it)->Client == client)
		{
			if(context1 == NULL)
			{
				context1 = *it;
			}
			else
			{
				context2 = *it;
				break;
			}
		}
		it++;
	}
	
	assert(context1 != NULL);
	assert(context2 != NULL);

	m_allContexts.remove(context1);
	m_allContexts.remove(context2);
	delete client, context1, context2;
}

} //namespace windows
} //namespace network
} //namespace Commom