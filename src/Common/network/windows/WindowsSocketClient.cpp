#include "WindowsSocketClient.h"

#include "WindowsSocketServer.h"
#include "../../Buffer.h"
#include "../../crypto/CryptEngine.h"
#include "WindowsCommonDefs.h"
#include "WindowsStartup.h"

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <string>

namespace Common {
namespace network {
namespace windows {

WindowsSocketClient::WindowsSocketClient(): m_server(NULL)
{

}

WindowsSocketClient::WindowsSocketClient(SOCKET socket, WindowsSocketServer* server)
{
	m_socket = socket;
	m_server = server;
}

void WindowsSocketClient::AddBytesToReveivedQueue(byte* bytes, size_t size)
{
	size_t pos = 0;
	while(pos < size)
	{
		m_receiveQueue.push(bytes[pos++]);
	}
	ProcessReceivedBytes();
}

size_t WindowsSocketClient::GetBytesFromSendQueue(byte* bytes, size_t maxSize)
{
	size_t ret = 0;

	while(ret < maxSize && m_sendQueue.size() > 0)
	{
		bytes[ret++] = m_sendQueue.front();
		m_sendQueue.pop();
	}

	return ret;
}

void WindowsSocketClient::SendPacket(packet::PacketBase& packet)
{
	Buffer_ptr buff = packet.GetProcessedBuffer();
	crypto::CryptEngine::GetInstance().XorCrypt(buff, packet.GetKey());
	SendBytes(buff->Data(), buff->Length());
}

void WindowsSocketClient::SendBytes(byte* bytes, size_t size)
{
	bool notify = m_sendQueue.size() > 0? false: true;
	
	for(size_t i = 0; i < size; i++)
	{
		m_sendQueue.push(bytes[i]);
	}

	if(notify)
	{
		if(m_server != NULL)
		{
			m_server->NotifyAvaliableBytesToSend(this);
		}
		else
		{
			ProcessSendQueue();
		}
	}
}

bool WindowsSocketClient::Connect(string address, string port)
{
	if(!WindowsStartup::GetInstance().IsInitialized())
	{
		if(!WindowsStartup::GetInstance().Initialize())
		{
			return false;
		}
	}

	struct addrinfo hints = {0};
	struct addrinfo *addr = NULL;

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_IP;

	if(getaddrinfo(address.c_str(), port.c_str(), &hints, &addr) != 0)
	{
		return false;
	}

	if(addr == NULL)
	{
		return false;
	}

	m_socket = WSASocket(addr->ai_family, addr->ai_socktype, addr->ai_protocol,
		NULL, 0, WSA_FLAG_OVERLAPPED);

	connect(m_socket, addr->ai_addr, addr->ai_addrlen);

	freeaddrinfo(addr);

	int zero = 0;
	if(setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char* )&zero, sizeof(zero)) == SOCKET_ERROR)
	{
		return false;
	}

	m_completionPort = CreateIoCompletionPort((HANDLE)m_socket, NULL, (ULONG_PTR)this, 1);
	if(m_completionPort == NULL)
	{
		return false;
	}

	m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadProc, this, 0, NULL);
	if(m_thread == NULL)
	{
		return false;
	}

	m_readClientContext = new ClientContext();
	m_writeClientContext = new ClientContext();

	m_readClientContext->State = CCT_RECEIVE;
	m_readClientContext->Client = this;
	m_readClientContext->WSBuffer.buf = (char*)m_readClientContext->Buffer;
	m_readClientContext->WSBuffer.len = TCP_BUFFER_SIZE;

	m_writeClientContext->State = CCT_SEND_NOT_WORK;
	m_writeClientContext->Client = this;

	return true;
}

void WindowsSocketClient::Desconect()
{
	closesocket(m_socket);
}

void WindowsSocketClient::ProcessSendQueue()
{
	size_t size = GetBytesFromSendQueue(m_writeClientContext->Buffer, TCP_BUFFER_SIZE);
	if(size > 0)
	{
		m_writeClientContext->WSBuffer.buf = (char*)m_writeClientContext->Buffer;
		m_writeClientContext->WSBuffer.len = size;
		m_writeClientContext->NumBytes = size;

		WSASend(m_writeClientContext->Client->GetSocket(), &m_writeClientContext->WSBuffer,
			1, &m_writeClientContext->NumBytes, 0, &m_writeClientContext->Overlapped, NULL);
		m_writeClientContext->State = CCT_SEND;
	}
}

DWORD WindowsSocketClient::WorkerThreadProc(WindowsSocketClient* pthis)
{
	return pthis->WorkerThread();
}

DWORD WindowsSocketClient::WorkerThread()
{
	LPWSAOVERLAPPED overlapped = NULL;
	WindowsSocketClient* client = NULL;
	BOOL bRet;
	DWORD numberOfBytes;
	DWORD flags = 0;
	int iRet;

	while(true)
	{

		bRet =  GetQueuedCompletionStatus(m_completionPort, &numberOfBytes,
											 (PDWORD_PTR)&client,
											 (LPOVERLAPPED*)&overlapped, 
											 INFINITE);
		
		

		if(bRet == FALSE || (bRet == TRUE && numberOfBytes == 0))
		{
			closesocket(m_socket);
			m_eventCallback(this, CEV_CLIENT_DESCONNECTED, NULL);
			
			continue;
		}

		if(client == NULL)
		{
			continue;
		}

		ClientContext* clientContext = &m_writeClientContext->Overlapped == overlapped? m_writeClientContext: m_readClientContext;

		if(clientContext->State == CCT_RECEIVE)
		{
			clientContext->Client->AddBytesToReveivedQueue(clientContext->Buffer, numberOfBytes);
			
			iRet = WSARecv(clientContext->Client->GetSocket(), &clientContext->WSBuffer,
				1, &clientContext->NumBytes, &flags, &clientContext->Overlapped, NULL);
			if( iRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()) )
			{
				closesocket(m_socket);
				m_eventCallback(this, CEV_CLIENT_DESCONNECTED, NULL);
			}
		}
		else
		{
			if(clientContext->NumBytes > numberOfBytes)
			{
				clientContext->WSBuffer.buf += numberOfBytes;
				clientContext->WSBuffer.len -= numberOfBytes;
				clientContext->NumBytes -= numberOfBytes;

				WSASend(clientContext->Client->GetSocket(), &clientContext->WSBuffer,
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

					WSASend(clientContext->Client->GetSocket(), &clientContext->WSBuffer,
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

} //namespace windows
} //namespace network
} //namespace Commom