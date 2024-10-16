#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "queue.h"
#include "netype.h"

typedef struct {
  int       sock;
  int       port;
  queue_t*  q_send;
  queue_t*  q_recv;
}network_t;

network_t*  net_create();
void        net_free();
void        net_send(chunk_t* data, ip4_addr src, ip4_addr dst);
chunk_t*    net_recv(ip4_addr *src, ip4_addr *dst);

void        net_running();

#endif //__NETWORK_H__
