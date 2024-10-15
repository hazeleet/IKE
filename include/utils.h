#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "chunk.h"

chunk_t* get_rand(int bytes);
uint64_t get_spi();

#endif //__UTILS_H__
