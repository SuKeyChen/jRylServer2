#include "pktUserLoginAck.h"

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

UserLoginAck::UserLoginAck(Buffer_ptr buff): PacketBase(buff) {

}

UserLoginAck::UserLoginAck(): PacketBase(PacketID, PacketLength) {

}

UserLoginAck::~UserLoginAck() {
    
}

void UserLoginAck::ProcessPacket() {

}

}
}
