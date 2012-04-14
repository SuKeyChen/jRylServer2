#ifndef _SERVERDETAILASK_H_
#define	_SERVERDETAILASK_H_

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

class ServerDetailAsk: public PacketBase{
public:
    ServerDetailAsk();
    virtual ~ServerDetailAsk();

    static const uint8 PacketID = 0x02;
    static const uint16 PacketLength = PACKET_HEADER_SIZE;
    
    //Packet struct
private:
	virtual void ProcessPacket();

};

}
}
#endif	//_SERVERDETAILASK_H_

