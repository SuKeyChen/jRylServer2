#ifndef _CRC32_H_
#define _CRC32_H_
#include "typedef.h"

namespace Common {

class Crc32 {
public:
	static uint32 Calculate(byte* bytes, size_t length);
private:
    Crc32(void);
};


} //namespace common
#endif