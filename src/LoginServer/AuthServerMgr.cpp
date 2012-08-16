#include "AuthServerMgr.h"

#include "../Common/typedef.h"
#include "../Common/stdDefs.h"
#include "../Common/Logger.h"
#include "../Common/utils.h"

#include "../Common/packet/pktPing.h"
#include "../Common/packet/pktAuthServer.h"
#include "../Common/packet/pktAuthServerAck.h"

#include "GlobalConfTable.h"
#include "AuthServer.h"

#include <boost/bind.hpp>

#include <ctime>

#define __LOG Common::Logger::GetInstance_ptr()

AuthServerMgr::AuthServerMgr():
	m_socketListen(new Common::network::SocketServer())
{	
	for(int i = 0;i < AUTHSERVER_SLOT_LENGTH; i++)
	{
		m_authServers[i] = NULL;
	}
}

bool AuthServerMgr::Start()
{
	GlobalConfTable* gconf = GlobalConfTable::GetInstance_ptr();

	m_socketListen->SetServerCallback(boost::bind(&AuthServerMgr::EventCallbakc, this, _1, _2, _3));
	if(!m_socketListen->BindAndListen(gconf->AuthServerListenAddress, Common::j_atoi<uint16>(gconf->AuthServerListenPort.c_str())))
	{
		__LOG->ShowError("Error start bind and listen address for AuthServers\n");
		return false;
	}
	return true;
}

void AuthServerMgr::EventCallbakc(Common::network::SocketServerBase* server, Common::network::ServerEventType type, void* arg)
{
	switch (type)
	{
	case Common::network::SEVT_CLIENT_CONNECTED:
		ClientConnect((Common::network::SocketClientBase*)arg);
		break;
	case Common::network::SEVT_CLIENT_DESCONNECTED:
		ClientDesconnect((Common::network::SocketClientBase*)arg);
		break;
	default:
		break;
	}
}

void AuthServerMgr::ClientConnect(Common::network::SocketClientBase* client)
{
	m_unauthorizedAuthServers[client].AttemptLogin = 0;
	m_unauthorizedAuthServers[client].State = UnauthorizedAuthServer::US_WAIT_PING;
	m_unauthorizedAuthServers[client].Time = ::time(NULL);
	client->SetEventCallback(boost::bind(&AuthServerMgr::EventCallbakcUnauthorized, this, _1, _2, _3));
}

void AuthServerMgr::EventCallbakcUnauthorized(Common::network::SocketClientBase* client, Common::network::ClientEventType type, void* arg)
{
	switch (type)
	{
	case Common::network::CEV_PACKET_RECEIVED:
		PacketParserUnauthorized(client, *(Common::Buffer_ptr*)arg);
		break;
	case Common::network::CEV_CLIENT_DESCONNECTED:
		ClientDesconnect(client);
		break;
	default:
		break;
	}
}

void AuthServerMgr::PacketParserUnauthorized(Common::network::SocketClientBase* client, Common::Buffer_ptr buff)
{
	GlobalConfTable* gconf = GlobalConfTable::GetInstance_ptr();
	buff->SetReaderOffset(0);
    
	switch (m_unauthorizedAuthServers[client].State) {
	case UnauthorizedAuthServer::US_WAIT_PING: {
		if(buff->Get<uint8>(1) != Common::packet::Ping::PacketID) {
			m_unauthorizedAuthServers.erase(client);
		    return;
		}
		m_unauthorizedAuthServers[client].State =  UnauthorizedAuthServer::US_WAIT_USERPASS;
		m_unauthorizedAuthServers[client].Time = ::time(NULL);
		break;
    }
	case UnauthorizedAuthServer::US_WAIT_USERPASS: {
		if(buff->Get<uint8>(1) != Common::packet::AuthServer::PacketID) {
			m_unauthorizedAuthServers.erase(client);
		    return;
		}
		Common::packet::AuthServer authServer(buff);
		Common::packet::AuthServerAck authServerAck;
		
		if(authServer.GetUser() == gconf->GlobalUser && authServer.GetPass() == gconf->GlobalPass) {
			uint8 slotnum = authServer.GetSlotNum();
		    if(m_authServers[slotnum] != NULL) {
		        bool slotfound = false;
		        for(slotnum++;slotnum < AUTHSERVER_SLOT_LENGTH;slotnum++) {
		            if(m_authServers[slotnum] == NULL) {
		                slotfound = true;
		                break;
		            }
		        }
		        if(!slotfound) {
		            for(slotnum = authServer.GetSlotNum();slotnum != 0xFF;slotnum--) {
		                if(m_authServers[slotnum] == NULL) {
		                    slotfound = true;
		                    break;
		                }
		            }
		        }
		        if(!slotfound) {
					m_unauthorizedAuthServers.erase(client);
					authServerAck.SetErro(Common::packet::AuthServerAck::EA_SERVER_FULL);
		            client->SendPacket(authServerAck);
		            return;
		        }
		    }

			m_unauthorizedAuthServers.erase(client);
			authServerAck.SetErro(Common::packet::AuthServerAck::EA_SUCESS);
			authServerAck.SetSlotNum(slotnum);
			client->SendPacket(authServerAck);
			m_authServers[slotnum] = new AuthServer(client, this);
		} else {
			Common::packet::AuthServerAck authServerAck;
			m_unauthorizedAuthServers.erase(client);
			if(m_unauthorizedAuthServers[client].AttemptLogin == MAX_AUTHSERVER_LOGIN_ATTEMPTS) {                    
				authServerAck.SetErro(Common::packet::AuthServerAck::EA_MAX_LOGIN_ATTEMPTS);					
		        client->SendPacket(authServerAck);
		    } else {
				authServerAck.SetErro(Common::packet::AuthServerAck::EA_USER_PASS_INCORRECT);
		        client->SendPacket(authServerAck);
				m_unauthorizedAuthServers[client].AttemptLogin++;
		    }
		}
		break;
    }
    default:
		m_unauthorizedAuthServers.erase(client);
        break;
    }
}

void AuthServerMgr::ClientDesconnect(Common::network::SocketClientBase* client)
{
	m_unauthorizedAuthServers.erase(client);
}

void AuthServerMgr::RemoveAuthServer(AuthServer* authServer)
{
	for(int i = 0;i < AUTHSERVER_SLOT_LENGTH; i++)
	{
		if(m_authServers[i] == authServer)
		{
			delete m_authServers[i];
			m_authServers[i] = NULL;
			return;
		}
	}
}

AuthServerMgr::~AuthServerMgr()
{
	std::map<Common::network::SocketClientBase*, UnauthorizedAuthServer>::iterator it = m_unauthorizedAuthServers.begin();
	while(it != m_unauthorizedAuthServers.end())
	{
		delete it->first;
		it++;
	}
	for(int i = 0;i < AUTHSERVER_SLOT_LENGTH; i++)
	{
		if(m_authServers[i] != NULL)
		{
			delete m_authServers[i];
		}
	}

	delete m_socketListen;
}