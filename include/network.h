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
void        net_free(network_t* net);
void        net_send(network_t* net, packet_t* pkt);
packet_t*   net_recv(network_t* net);

void        net_running(network_t* net);

#endif //__NETWORK_H__
