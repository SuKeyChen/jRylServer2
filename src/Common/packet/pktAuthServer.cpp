#include "pktAuthServer.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

AuthServer::AuthServer(Buffer_ptr buff): PacketBase(buff) {
	m_slotNum = m_Buffer->Get<uint8>();
	m_user = m_Buffer->GetString<string>();
	m_pass = m_Buffer->GetString<string>();
}

AuthServer::AuthServer(): PacketBase(PacketID) { }

AuthServer::~AuthServer() { }

void AuthServer::ProcessPacket() {
	m_Buffer->Add(m_slotNum);
	m_Buffer->AddString(m_user);
	m_Buffer->AddString(m_pass);
}

}
}
