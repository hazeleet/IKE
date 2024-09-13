#include "daemon.h"

daemon_t IKE;

void daemon_create()
{
	IKE.log = log_create();
	IKE.cfg = cfg_create("conf/sample.conf");
	IKE.net = net_create();
}

void daemon_running()
{
	char src[IPSTR_LEN], dst[IPSTR_LEN];
	while(1) {
		packet_t* pkt = net_recv(IKE.net);
		net_atos(pkt->src, src, IPSTR_LEN);
		net_atos(pkt->dst, dst, IPSTR_LEN);
		logging(DBG, "[DMN] Receive %d-byte packet(%s->%s)\n",
				pkt->data->size, src, dst);
		// Traveling sas
	}
}
