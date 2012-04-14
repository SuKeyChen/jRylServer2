#ifndef _SERVERLISTASK_H_
#define	_SERVERLISTASK_H_

#include "../Buffer.h"
#include "PacketBase.h"
#include "../typedef.h"
#include "../Logger.h"

namespace Common {
namespace packet {

class ServerListAsk: public PacketBase{
public:
    ServerListAsk(Buffer_ptr buff);
	ServerListAsk();
    virtual ~ServerListAsk();

    static const uint8 PacketID = 0x56;
    static const uint16 PacketLength = 16;

	inline uint32 GetClientVersion();
	inline uint32 GetPatchModel();

	inline void SetClientVersion(uint32 clientVersion);
	inline void SetPatchModel(uint32 patchModel);

    //Packet struct
	//uint32 ClientVersion;
private:
	virtual void ProcessPacket();

	uint32 m_clientVersion;
};

inline uint32 ServerListAsk::GetClientVersion() {
	return m_clientVersion;
}

inline uint32 ServerListAsk::GetPatchModel() {
	return m_Status;
}

inline void ServerListAsk::SetClientVersion(uint32 clientVersion) {
	m_clientVersion = clientVersion;
}

inline void ServerListAsk::SetPatchModel(uint32 patchModel) {
	m_Status = patchModel;
}

}
}

#endif	/* _SERVERLISTASK_H_ */

