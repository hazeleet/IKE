#include "netype.h"

#include <stdlib.h>
#include <stdio.h>

packet_t* pkt_create()
{
	packet_t* pkt = calloc(1, sizeof(packet_t));

	return pkt;
}

void pkt_free(packet_t* pkt)
{
	if(pkt->data)
		chk_free(pkt->data);
	free(pkt);
}

ip4_addr net_stoa(const char* ipstr)
{
	uint32_t a,b,c,d;

	if(sscanf(ipstr, "%u.%u.%u.%u", &a, &b, &c, &d) == 4) {
		ip4_addr result = d + (c<<8) + (b<<16) + (a<<24);
		return result;
	}

	return 0;
}

void net_atos(ip4_addr addr, char* ipstr, int ipstr_len)
{
	snprintf(ipstr, ipstr_len, "%u.%u.%u.%u",
			(addr) & 0xFF,
			(addr>>8) & 0xFF,
			(addr>>16) & 0xFF,
			(addr>>24) & 0xFF);
}
