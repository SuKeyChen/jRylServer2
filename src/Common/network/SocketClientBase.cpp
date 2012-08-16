#include "SocketClientBase.h"

#include "../stdDefs.h"
#include "../crypto/CryptEngine.h"

namespace Common {
namespace network {

SocketClientBase::SocketClientBase(): m_eventCallback(NULL), m_processingBuffer(new Buffer())
{

}

void SocketClientBase::ProcessReceivedBytes()
{
	if(m_processingBuffer->Length() > 0)
	{
		uint16 packetSize = m_processingBuffer->Get<uint16>(2);
		while(m_processingBuffer->Length() < packetSize && m_receiveQueue.size() > 0)
		{
			m_processingBuffer->Add(m_receiveQueue.front());
			m_receiveQueue.pop();
		}

		if(m_processingBuffer->Length() == packetSize)
		{
			if(m_eventCallback != NULL)
			{
				m_eventCallback(this, CEV_PACKET_RECEIVED, &m_eventCallback);
			}
			m_processingBuffer = Buffer_ptr(new Buffer());
		}		

		if(m_receiveQueue.size() > 0)
		{
			ProcessReceivedBytes();
		}
	}
	else
	{
		if(m_receiveQueue.size() > 11)
		{
			for(int i = 0; i < 12; i++)
			{
				m_processingBuffer->Add(m_receiveQueue.front());
				m_receiveQueue.pop();
			}

			crypto::CryptEngine::GetInstance_ptr()->XorDecryptPacketHeader(m_processingBuffer->Data());

			uint16 packetSize = m_processingBuffer->Get<uint16>(2) & ~(PACKET_TYPE_NORMAL | PACKET_TYPE_COMPRESSED);
			while(m_processingBuffer->Length() < packetSize && m_receiveQueue.size() > 0)
			{
				m_processingBuffer->Add(m_receiveQueue.front());
				m_receiveQueue.pop();
			}

			if(m_processingBuffer->Length() == packetSize)
			{
				crypto::CryptEngine::Cryptkey key;
				m_processingBuffer->GetPack(key, 4);
				crypto::CryptEngine::GetInstance_ptr()->XorDecryptPacketBody(m_processingBuffer->Data(), m_processingBuffer->Length(), key);
				if(m_eventCallback != NULL)
				{
					m_eventCallback(this, CEV_PACKET_RECEIVED, &m_processingBuffer);
				}
				m_processingBuffer = Buffer_ptr(new Buffer());
			}			

			if(m_receiveQueue.size() > 0)
			{
				ProcessReceivedBytes();
			}		
		}
	}
}

}
}