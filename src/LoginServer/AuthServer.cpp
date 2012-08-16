#include "AuthServer.h"

#include "../Common/network/SocketClient.h"

#include "../Common/packet/pktPing.h"
#include "../Common/packet/pktServerDetail.h"
#include "../Common/packet/pktServerDetailAsk.h"
#include "../Common/utils.h"
#include "AuthServerMgr.h"

#include <boost/bind.hpp>

using namespace Common;

#define __LOG Common::Logger::GetInstance_ptr()

AuthServer::AuthServer(Common::network::SocketClientBase* client, AuthServerMgr* authServerMgr):
	m_socketClient(client),	m_authServerMgr(authServerMgr), m_status(ASS_WAIT_DETAILS)
{
	m_socketClient->SetEventCallback(boost::bind(&AuthServer::EventCallback, this, _1, _2, _3));

	m_socketClient->SendPacket(packet::ServerDetailAsk());
}

void AuthServer::EventCallback(Common::network::SocketClientBase* client, Common::network::ClientEventType type, void* arg)
{
	switch (type)
	{
	case Common::network::CEV_PACKET_RECEIVED:
		PacketParser(*(Common::Buffer_ptr*)arg);
		break;
	case Common::network::CEV_CLIENT_DESCONNECTED:
		ClientDesconnect();
		break;
	default:
		break;
	}
}

void AuthServer::PacketParser(Common::Buffer_ptr buff)
{
	buff->SetReaderOffset(0);
	switch(buff->Get<uint8>(1)) {
	case Common::packet::Ping::PacketID:
        m_lastPing = Common::packet::Ping(buff).GetPingValue();
		break;
	case Common::packet::ServerDetail::PacketID:
		if(m_status == ASS_WAIT_DETAILS) {
            Common::packet::ServerDetail serverDetail(buff);
            m_group = serverDetail.GetGroup();
            m_name = serverDetail.GetName();
			m_address = serverDetail.GetAddress();
			m_status = ASS_OK;
			__LOG->ShowInfo("Connected AuthServer(%s) with ip address %s\n", m_name.c_str(), Common::IpToStr(m_address));
        }
		break;
    }
}
void AuthServer::ClientDesconnect()
{
	m_authServerMgr->RemoveAuthServer(this);
}
AuthServer::~AuthServer()
{
}