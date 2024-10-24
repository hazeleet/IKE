#ifndef __SA_H__
#define __SA_H__

#include "netype.h"
#include "ike_protocol.h"
#include "crypto.h"
#include "exchange.h"

typedef enum {
  INIT = 0,
  INIT_ESTABLISHED,
}sa_state;

typedef struct sa_t sa_t;
struct sa_t {
  struct {
    ip4_addr addr;
  }left, right;

	sa_state	state;
  bool      is_initiator;
  uint64_t  SPIi, SPIr;
  uint32_t  message_id;
  uint8_t   major_version, minor_version;
  ike_exchange_type last_exchange;

  chunk_t   *Ni, *Nr;
	crypto_t	crypto;

  sa_t*   prev;
  sa_t*   next;
};

sa_t*		sa_create();
void		sa_push(sa_t* sa);

void		sa_process(sa_t* this, chunk_t* data);

#endif //__SA_H__
