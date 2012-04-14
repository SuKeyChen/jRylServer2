#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
#include <boost/cstdint.hpp>

namespace Common {
namespace math {
class Half;
}//namespace math
}//namespace common

typedef boost::int64_t int64;
typedef boost::int32_t int32;
typedef boost::int16_t int16;
typedef boost::int8_t int8;
typedef boost::uint64_t uint64;
typedef boost::uint32_t uint32;
typedef boost::uint16_t uint16;
typedef boost::uint8_t uint8;
typedef Common::math::Half f16;
typedef float f32;
typedef double f64;
typedef uint8 byte;

#endif