#include "sa.h"
#include "daemon.h"
#include "utils.h"
#include "exchange.h"

#include <stdlib.h>

void _ike_sa_init_i(sa_t* sa);

sa_t* sa_create()
{
	sa_t* this = calloc(1, sizeof(sa_t));

	return this;
}

void sa_push(sa_t* sa)
{
	sa->next = IKE.sas;
	if(IKE.sas)
		IKE.sas->prev = sa;
	IKE.sas = sa;

	// IKE_SA_INIT make&send
	_ike_sa_init_i(sa);
}

void _ike_sa_init_i(sa_t* sa)
{
	sa->is_initiator = true;
	sa->SPIi = get_spi();
	sa->message_id = 0;

	exchange_t* exg = exg_create();
	exg->header.SPIi = sa->SPIi;

	// SA
	// KE
	// Nx
	// send
	chunk_t* packed = exg_pack(exg);
	net_send(packed, sa->left.addr, sa->right.addr);
	// change state
}
