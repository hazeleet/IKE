#include "daemon.h"
#include <pthread.h>

daemon_t IKE;

void*	_running();

void daemon_create()
{
	IKE.log = log_create();
	IKE.sam = sam_create();
	IKE.cfg = cfg_create("conf/sample.conf");
	IKE.net = net_create();
}

void*	_running()
{
	char src[IPSTR_LEN], dst[IPSTR_LEN];
	while(1) {
		packet_t* pkt = net_recv(IKE.net);
		net_atos(pkt->src, src, IPSTR_LEN);
		net_atos(pkt->dst, dst, IPSTR_LEN);
		logging(DBG, "[DMN] Receive %d-byte packet(%s->%s)\n",
				pkt->data->size, src, dst);
		sam_match(IKE.sam, pkt);
	}
}

void daemon_running()
{
	pthread_t tid;
	pthread_create(&tid, NULL, _running, NULL);

	for(cfg_peer_t* peer = IKE.cfg->peers;
			peer != NULL;
			peer = peer->next) {
		sa_t* sa = sa_create();
		sa->left.addr = peer->left.addr;
		sa->right.addr = peer->right.addr;
		sam_push(IKE.sam, sa);
	}
}
