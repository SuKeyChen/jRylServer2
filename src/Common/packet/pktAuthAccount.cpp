#include "pktAuthAccount.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

AuthAccount::AuthAccount(): PacketBase(PacketID, PacketLength) { }

AuthAccount::AuthAccount(Buffer_ptr buff): PacketBase(buff) {
    m_userAccount = m_Buffer->GetStringSizeFixed<string>(16);
    m_userPassWord = m_Buffer->GetStringSizeFixed<string>(33);
    m_sessionID = m_Buffer->Get<uint32>();
    m_clientVersion = m_Buffer->Get<uint32>();
    m_clientCheckSum =  m_Buffer->Get<uint32>();
    m_flag =  m_Buffer->Get<uint16>();
}

void AuthAccount::ProcessPacket() {
	m_Buffer->AddStringSizeFixed(m_userAccount, 16);
	m_Buffer->AddStringSizeFixed(m_userPassWord, 33);
    m_Buffer->Add(m_sessionID);
    m_Buffer->Add(m_clientVersion);
    m_Buffer->Add(m_clientCheckSum);
    m_Buffer->Add(m_flag);
}

AuthAccount::~AuthAccount() {
    
}

}
}
