#include "pktServerListAsk.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

ServerListAsk::ServerListAsk(Buffer_ptr buff): PacketBase(buff) {
	m_clientVersion = m_Buffer->Get<uint32>();
}

ServerListAsk::ServerListAsk(): PacketBase(PacketID, PacketLength) { }

void ServerListAsk::ProcessPacket() {
	m_Buffer->Add(m_clientVersion);
}

ServerListAsk::~ServerListAsk() { }

}
}
