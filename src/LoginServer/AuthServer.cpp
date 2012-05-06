#include "AuthServer.h"

#include "../Common/network/SocketClient.h"

#include "../Common/packet/pktPing.h"
#include "../Common/packet/pktServerDetail.h"
#include "../Common/packet/pktServerDetailAsk.h"
#include "../Common/utils.h"
#include "AuthServerMgr.h"

#include <boost/bind.hpp>

#define __LOG Common::Logger::GetInstance_ptr()

AuthServer::AuthServer(Common::network::SocketClient* client, AuthServerMgr* authServerMgr):
	m_socketClient(client),	m_authServerMgr(authServerMgr), m_status(ASS_WAIT_DETAILS)
{
	m_socketClient->SetCloseCallback(boost::bind(&AuthServer::ClientDesconnect, this, _1));
	m_socketClient->SetReceivePacketCallback(boost::bind(&AuthServer::PacketParser, this, _1, _2));
	m_socketClient->SendPacket(Common::packet::ServerDetailAsk());
}

void AuthServer::PacketParser(Common::network::SocketClient* client, Common::Buffer_ptr buff)
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
void AuthServer::ClientDesconnect(Common::network::SocketClient* client)
{
	m_authServerMgr->RemoveAuthServer(this);
}
AuthServer::~AuthServer()
{
	delete m_socketClient;
}