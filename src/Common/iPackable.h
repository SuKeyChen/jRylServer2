#ifndef PACKABLE_H
#define PACKABLE_H

#include <cstddef>

#include <boost/shared_ptr.hpp>
#include "typedef.h"


namespace Common {

class Buffer;
typedef boost::shared_ptr<Buffer> Buffer_ptr;

class Packable {
public:
    virtual void Pack(Buffer_ptr buffer) = 0;
    virtual void Unpack(Buffer_ptr buffer) = 0;
};

} //namespace common

#endif