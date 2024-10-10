#ifndef __SAMANAGER_H__
#define __SAMANAGER_H__

#include "netype.h"
#include "protocol.h"

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
  chunk_t   *SPIi, *SPIr;
  uint32_t  message_id;
  uint8_t   major_version, minor_version;
  exchange_type last_exchange;

  sa_t*   prev;
  sa_t*   next;
};

typedef struct {
  sa_t*   sas;
}samanager_t;

samanager_t*  sam_create();
void          sam_push(samanager_t* sam, sa_t* sa);
void          sam_match(samanager_t* sam, packet_t* pkt);

sa_t*         sa_create();

#endif //__SAMANAGER_H__
