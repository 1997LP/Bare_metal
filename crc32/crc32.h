#ifndef __CRC__
#define __CRC__

#include <stdint.h>

uint32_t crc32(uint32_t crc, const unsigned char *p, unsigned int len);

#endif