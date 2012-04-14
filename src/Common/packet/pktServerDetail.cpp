#include "pktServerDetail.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

ServerDetail::ServerDetail(Buffer_ptr buff): PacketBase(buff) {
    m_group = m_Buffer->Get<uint8>();
    m_name = m_Buffer->GetString<string>();
    m_address = m_Buffer->Get<uint32>();
}

void ServerDetail::ProcessPacket() {
	m_Buffer->Add(m_group);
	m_Buffer->AddString(m_name);
	m_Buffer->Add(m_address);
}

ServerDetail::ServerDetail(): PacketBase(PacketID) { }
ServerDetail::~ServerDetail() { }

}
}