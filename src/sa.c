#include "sa.h"
#include "daemon.h"
#include "utils.h"
#include "exchange.h"
#include "proposal.h"

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
	sa->Ni = get_rand(20);
	sa->DH.group = MODP_1024;
	sa->DH.key = get_rand(1024/8);

	exchange_t* exg = exg_create();
	exg->header.SPIi = sa->SPIi;
	exg->header.exchange_type = IKE_SA_INIT;

	// SA
	proposal_t* proposals = ppl_create(PROTOCOL_IKE);
	ppl_set_encr(proposals, ENCR_AES_CBC, 128);
	ppl_set_encr(proposals, ENCR_AES_CBC, 256);
	ppl_set_integ(proposals, AUTH_HMAC_SHA1_96);
	ppl_set_prf(proposals, PRF_HMAC_SHA1);
	ppl_set_dh(proposals, MODP_1024);
	payload_t* SA = pld_create(PLD_SA);
	pld_sa_set(SA, proposals);
	exg_push(exg, SA);
	// KE
	payload_t* KE = pld_create(PLD_KE);
	pld_ke_set(KE, sa->DH.group, sa->DH.key);
	exg_push(exg, KE);
	// Nx
	payload_t* Ni = pld_create(PLD_Nx);
	pld_nx_set(Ni, sa->Ni);
	exg_push(exg, Ni);
	// send
	chunk_t* packed = exg_pack(exg);
	net_send(packed, sa->left.addr, sa->right.addr);
	// change state
}
