#include "pktAuthServerAck.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

AuthServerAck::AuthServerAck(): PacketBase(PacketID, PacketLength) { }

AuthServerAck::AuthServerAck(Buffer_ptr buff): PacketBase(buff) {
	m_slotNum = m_Buffer->Get<uint8>();
}

AuthServerAck::~AuthServerAck() {
    
}

void AuthServerAck::ProcessPacket() {
	m_Buffer->Add(m_slotNum);
}

}
}