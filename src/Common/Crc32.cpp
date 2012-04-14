#include "Crc32.h"
#include "Crc32.tab"

#include "typedef.h"

namespace Common {

	uint32 Crc32::Calculate(byte* bytes, size_t length) {
        uint32 crc = 0xffffffff;
        while (length--) {
            crc = (crc >> 8) ^ Table[(crc & 0xFF) ^ *bytes++];
        }    
        return (crc ^ 0xffffffff);
    }

} //namespace common