#ifndef __EXCHANGE_H__
#define __EXCHANGE_H__

#include "ike_protocol.h"
#include "payload.h"
#include "chunk.h"

typedef struct {
  ike_header_t*   header;
  payload_t*      payloads;
}exchange_t;

exchange_t* exg_create();
void        exg_push(exchange_t* exg, payload_t* pld);
chunk_t*    exg_pack(exchange_t* exg);

#endif //__EXCHANGE_H__
