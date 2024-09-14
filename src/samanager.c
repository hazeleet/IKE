#include "samanager.h"
#include "daemon.h"

#include <stdlib.h>

samanager_t* sam_create()
{
	samanager_t* sam = calloc(1, sizeof(samanager_t));

	return sam;
}

void sam_push(samanager_t* sam, sa_t* sa)
{
	sa->next = sam->sas;
	if(sam->sas)
		sam->sas->prev = sa;
	sam->sas = sa;
}

sa_t* sa_create()
{
	sa_t* sa = calloc(1, sizeof(sa_t));

	return sa;
}

void sam_match(samanager_t* sam, packet_t* pkt)
{
	char left[IPSTR_LEN], right[IPSTR_LEN];
	for(sa_t* sa = sam->sas; sa != NULL; sa = sa->next) {
		if(sa->left.addr == pkt->dst && sa->right.addr == pkt->src) {
			net_atos(sa->left.addr, left, IPSTR_LEN);
			net_atos(sa->right.addr, right, IPSTR_LEN);
			logging(DBG, "[SAM] Matched %s:%s\n", left, right);
			return;
		}
	}
	net_atos(pkt->dst, left, IPSTR_LEN);
	net_atos(pkt->src, right, IPSTR_LEN);
	logging(DBG, "[SAM] NotMatched %s:%s\n", left, right);
}
