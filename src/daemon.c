#include "daemon.h"
#include "exchange.h"
#include <pthread.h>

daemon_t IKE;

void*	_running();

void daemon_create(daemon_args_t args)
{
	IKE.log = log_create();
	IKE.sas = NULL;
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
		for(sa_t* sa = IKE.sas; sa != NULL; sa = sa->next) {
			if(sa->left.addr == dst && sa->right.addr == src) {
				logging(DBG, "[DMN] Matched %s:%s\n", bsrc, bdst);
				exchange_t* exg = exg_unpack(data);
				logging(DBG, "[DWN] unpacking\n");
				logging(DBG, "      SPIi: 0x%llX\n", exg->header.SPIi);
				logging(DBG, "      SPIr: 0x%llX\n", exg->header.SPIr);
				logging(DBG, "      next pld: %d\n", exg->header.next_payload);
				logging(DBG, "      version: %d\n", exg->header.version);
				logging(DBG, "      type: %d\n", exg->header.exchange_type);
				logging(DBG, "      flags: 0x%X\n", exg->header.flags);
				logging(DBG, "      message id: %d\n", exg->header.message_id);
				logging(DBG, "      length: %d\n", exg->header.length);
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
		sa_push(sa);
	}
}
