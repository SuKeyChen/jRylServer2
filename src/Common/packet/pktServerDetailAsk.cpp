#include "pktServerDetailAsk.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

ServerDetailAsk::ServerDetailAsk(): PacketBase(PacketID, PacketLength) { }

ServerDetailAsk::~ServerDetailAsk() { }

void ServerDetailAsk::ProcessPacket() { }

}
}
