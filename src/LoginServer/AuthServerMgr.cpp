#include "AuthServerMgr.h"

#include "../Common/typedef.h"
#include "../Common/stdDefs.h"
#include "../Common/Logger.h"

#include "../Common/packet/pktPing.h"
#include "../Common/packet/pktAuthServer.h"
#include "../Common/packet/pktAuthServerAck.h"

#include "GlobalConfTable.h"
#include "AuthServer.h"

#include <boost/bind.hpp>

#include <ctime>

#define __LOG Common::Logger::GetInstance_ptr()

AuthServerMgr::AuthServerMgr()
{	
	for(int i = 0;i < AUTHSERVER_SLOT_LENGTH; i++)
	{
		m_authServers[i] = NULL;
	}
}

bool AuthServerMgr::Start()
{
	GlobalConfTable* gconf = GlobalConfTable::GetInstance_ptr();

	m_socketListen.SetAcceptCallback(boost::bind(&AuthServerMgr::ClientConnect, this, _1));
	if(!m_socketListen.BindAndListen(gconf->AuthServerListenAddress, gconf->AuthServerListenPort))
	{
		__LOG->ShowError("Error start bind and listen address for AuthServers\n");
		return false;
	}
	return true;
}

void AuthServerMgr::ClientConnect(Common::network::SocketClient* client)
{
	m_unauthorizedAuthServers[client].AttemptLogin = 0;
	m_unauthorizedAuthServers[client].State = UnauthorizedAuthServer::US_WAIT_PING;
	m_unauthorizedAuthServers[client].Time = ::time(NULL);
	client->SetCloseCallback(boost::bind(&AuthServerMgr::ClientDesconnect, this, _1));
	client->SetReceivePacketCallback(boost::bind(&AuthServerMgr::PacketParserUnauthorized, this, _1, _2));
}

void AuthServerMgr::PacketParserUnauthorized(Common::network::SocketClient* client, Common::Buffer_ptr buff)
{
	GlobalConfTable* gconf = GlobalConfTable::GetInstance_ptr();
	buff->SetReaderOffset(0);
    
	switch (m_unauthorizedAuthServers[client].State) {
	case UnauthorizedAuthServer::US_WAIT_PING: {
		if(buff->Get<uint8>(1) != Common::packet::Ping::PacketID) {
			m_unauthorizedAuthServers.erase(client);
		    delete client;
		    return;
		}
		m_unauthorizedAuthServers[client].State =  UnauthorizedAuthServer::US_WAIT_USERPASS;
		m_unauthorizedAuthServers[client].Time = ::time(NULL);
		break;
    }
	case UnauthorizedAuthServer::US_WAIT_USERPASS: {
		if(buff->Get<uint8>(1) != Common::packet::AuthServer::PacketID) {
			m_unauthorizedAuthServers.erase(client);
		    delete client;
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
		            delete client;
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
		        delete client;
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
        delete client;
        break;
    }
}

void AuthServerMgr::ClientDesconnect(Common::network::SocketClient* client)
{
	m_unauthorizedAuthServers.erase(client);
	delete client;
}

void AuthServerMgr::RemoveAuthServer(AuthServer* authServer)
{
	for(int i = 0;i < AUTHSERVER_SLOT_LENGTH; i++)
	{
		if(m_authServers[i] == authServer)
		{
			delete m_authServers[i];
			return;
		}
	}
}

AuthServerMgr::~AuthServerMgr()
{
	std::map<Common::network::SocketClient*, UnauthorizedAuthServer>::iterator it = m_unauthorizedAuthServers.begin();
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
}