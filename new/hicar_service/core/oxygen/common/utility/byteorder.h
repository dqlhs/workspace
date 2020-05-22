#pragma once

#if defined(LINUX)
#include <endian.h>
#elif defined(FREEBSD)
#include <sys/endian.h>
#else
// TEST IF THERE COULD BE AN ERROR!
//#error "Undefined OS (only Linux/FreeBSD are currently supported)"
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define OXYGEN_BYTES_TO_WORD(x0, x1) (uint16_t((x0) << 8 | (x1)))
#define OXYGEN_BYTES_TO_LONG(x0, x1, x2, x3) (uint32_t((x0) << 24 | (x1) << 16 | (x2) << 8 | (x3)))

#define OXYGEN_WORDS_TO_LONG(x0, x1) (uint32_t((x0) << 16 | (x1)))

#define OXYGEN_WORD_BYTE0(x) (uint8_t((x) & 0xFF))
#define OXYGEN_WORD_BYTE1(x) (uint8_t((x) >> 8))

#define OXYGEN_LONG_BYTE0(x) (uint8_t((x) & 0xFF))
#define OXYGEN_LONG_BYTE1(x) (uint8_t(((x) >> 8) & 0xFF))
#define OXYGEN_LONG_BYTE2(x) (uint8_t(((x) >> 16) & 0xFF))
#define OXYGEN_LONG_BYTE3(x) (uint8_t(((x) >> 24) & 0xFF))

#define OXYGEN_LONG_WORD0(x) (uint16_t((x) & 0xFFFF))
#define OXYGEN_LONG_WORD1(x) (uint16_t(((x) >> 16) & 0xFFFF))

#elif __BYTE_ORDER == __BIG_ENDIAN

#define OXYGEN_BYTES_TO_WORD(x0, x1) (uint16_t((x1) << 8 | (x0)))
#define OXYGEN_BYTES_TO_LONG(x0, x1, x2, x3) (uint32_t((x3) << 24 | (x2) << 16 | (x1) << 8 | (x0)))

#define OXYGEN_WORD_BYTE0(x) (uint8_t((x) >> 8))
#define OXYGEN_WORD_BYTE1(x) (uint8_t((x) & 0xFF))

#define OXYGEN_LONG_BYTE0(x) (uint8_t(((x) >> 24) & 0xFF))
#define OXYGEN_LONG_BYTE1(x) (uint8_t(((x) >> 16) & 0xFF))
#define OXYGEN_LONG_BYTE2(x) (uint8_t(((x) >> 8) & 0xFF))
#define OXYGEN_LONG_BYTE3(x) (uint8_t((x) & 0xFF))

#define OXYGEN_LONG_WORD0(x) (uint16_t((((x) >> 16) & 0xFFFF))
#define OXYGEN_LONG_WORD1(x) (uint16_t(((x) & 0xFFFF))

#else

#error "__BYTE_ORDER is not defined!"

#endif
