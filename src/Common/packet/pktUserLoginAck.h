#ifndef _USERLOGINACK_H_
#define	_USERLOGINACK_H_

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

class UserLoginAck: public PacketBase{
public:
    UserLoginAck(Buffer_ptr buff);
	UserLoginAck();
    virtual ~UserLoginAck();

    static const uint8 PacketID = 0x59;
    static const uint16 PacketLength = 488;
private:
	virtual void ProcessPacket();

};

}
}

#endif	/* _USERLOGINACK_H_ */