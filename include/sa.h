#ifndef __SA_H__
#define __SA_H__

#include "netype.h"
#include "ike_protocol.h"

typedef enum {
  INIT = 0,
  WAIT_INIT_RES,
  WAIT_AUTH_RES,
}sa_state;

typedef struct sa_t sa_t;
struct sa_t {
  struct {
    ip4_addr addr;
  }left, right;

  bool      is_initiator;
  uint64_t  SPIi, SPIr;
  uint32_t  message_id;
  uint8_t   major_version, minor_version;
  ike_exchange_type last_exchange;

  chunk_t   *Ni, *Nr;

  sa_t*   prev;
  sa_t*   next;
};

sa_t*  sa_create();
void   sa_push(sa_t* sa);

#endif //__SA_H__
