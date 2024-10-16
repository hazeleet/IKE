#include "daemon.h"
#include <pthread.h>

daemon_t IKE;

void*	_running();

void daemon_create(daemon_args_t args)
{
	IKE.log = log_create();
	IKE.sam = sam_create();
	IKE.cfg = cfg_create(args.conf_file);
	IKE.net = net_create();
}

void*	_running()
{
	ip4_addr src, dst;
	char bsrc[IPSTR_LEN], bdst[IPSTR_LEN];
	while(1) {
		chunk_t* data = net_recv(&src, &dst);
		net_atos(src, bsrc, IPSTR_LEN);
		net_atos(dst, bdst, IPSTR_LEN);
		logging(DBG, "[DMN] Receive %d-byte packet(%s->%s)\n",
				data->size, bsrc, bdst);
		// Match
		for(sa_t* sa = IKE.sam->sas; sa != NULL; sa = sa->next) {
			if(sa->left.addr == dst && sa->right.addr == src) {
				logging(DBG, "[SAM] Matched %s:%s\n", bsrc, bdst);
				// switch by state
			}
		}
		//logging(DBG, "[SAM] NotMatched %s:%s\n", bsrc, bdst);
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
