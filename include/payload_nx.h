#ifndef __PAYLOAD_NX_H__
#define __PAYLOAD_NX_H__

#include "chunk.h"

typedef struct {
  chunk_t* nonce;
}payload_nx_t;

payload_nx_t* pld_nx_create(int nonce_size);

#endif //__PAYLOAD_NX_H__
