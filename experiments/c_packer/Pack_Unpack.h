#ifndef PACK_UNPACK_H_
#define PACK_UNPACK_H_

#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

// various bits for floating point types--
// varies for different architectures
typedef float float32_t;
typedef double float64_t;

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

int32_t pack(unsigned char *buf, char *format, ...);
void unpack(unsigned char *buf, char *format, ...);

#endif
