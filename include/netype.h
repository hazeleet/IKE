#ifndef __NETYPE_H__
#define __NETYPE_H__

#include "chunk.h"

#include <stdint.h>

typedef uint32_t ip4_addr;
#define IPSTR_LEN   4*3+4

typedef struct {
	chunk_t*	data;
	ip4_addr	src;
	ip4_addr	dst;
}packet_t;

packet_t*   pkt_create();
void				pkt_free(packet_t* pkt);

ip4_addr    net_stoa(const char* ipstr);
void        net_atos(ip4_addr addr, char* ipstr, int ipstr_len);


#endif //__NETYPE_H__
