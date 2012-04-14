#include "pktPing.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

Ping::Ping(Buffer_ptr buff): PacketBase(buff) {
    m_pingValue = m_Buffer->Get<uint32>();
}

Ping::Ping(): PacketBase(PacketID, PacketLength) { }

void Ping::ProcessPacket() {
	m_Buffer->Add(m_pingValue);
}

Ping::~Ping() { }

}
}
