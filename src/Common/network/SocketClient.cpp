#include "SocketClient.h"

#include "SocketServer.h"
#include "../Logger.h"
#include "../crypto/CryptEngine.h"
#include "../stdDefs.h"


namespace Common {
namespace network {

SocketClient::SocketClient():
m_socketServer(NULL), m_readQueue(10)
{
}

bool SocketClient::Connect(string address, string port)
{
    return true;
}

void SocketClient::SendPacket(packet::PacketBase& packet)
{
	Buffer_ptr buff = packet.GetProcessedBuffer();

	crypto::CryptEngine::GetInstance().XorCrypt(buff, packet.GetKey());
}

void SocketClient::Close()
{
}

SocketClient::~SocketClient()
{
}

}
}