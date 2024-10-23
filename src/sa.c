#include "sa.h"
#include "daemon.h"
#include "utils.h"
#include "exchange.h"
#include "proposal.h"

#include <stdlib.h>

void _ike_sa_init_i(sa_t* sa);

void make_ike_sa_init_request(sa_t* this);

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
	make_ike_sa_init_request(sa);
}

void make_ike_sa_init_request(sa_t* this)
{
	this->is_initiator = true;
	this->SPIi = get_spi();
	this->message_id = 0;
	this->Ni = get_rand(20);
	this->crypto.DH.group = MODP_1024;
	this->crypto.DH.key = get_rand(1024/8);

	exchange_t* exg = exg_create();
	exg->header.SPIi = this->SPIi;
	exg->header.exchange_type = IKE_SA_INIT;

	// SA
	proposal_t* proposals = ppl_create(PROTOCOL_IKE);
	ppl_set_encr(proposals, ENCR_AES_CBC, 128);
	ppl_set_encr(proposals, ENCR_AES_CBC, 256);
	ppl_set_integ(proposals, AUTH_HMAC_SHA1_96);
	ppl_set_prf(proposals, PRF_HMAC_SHA1);
	ppl_set_dh(proposals, this->crypto.DH.group);
	payload_t* SA = pld_create(PLD_SA);
	pld_sa_set(SA, proposals);
	exg_push(exg, SA);
	// KE
	payload_t* KE = pld_create(PLD_KE);
	pld_ke_set(KE, this->crypto.DH.group, this->crypto.DH.key);
	exg_push(exg, KE);
	// Nx
	payload_t* Ni = pld_create(PLD_Nx);
	pld_nx_set(Ni, this->Ni);
	exg_push(exg, Ni);
	// send
	chunk_t* packed = exg_pack(exg);
	net_send(packed, this->left.addr, this->right.addr);
	// change state
	this->state = WAIT_INIT_RES;
}

void sa_process_init_request(sa_t* this, exchange_t* exchange)
{
	this->is_initiator = false;
	this->SPIi = exchange->header.SPIi;
	this->SPIr = get_spi();
	this->message_id = exchange->header.message_id+1;

	// choose proposal

}

void sa_process(sa_t* this, chunk_t* data)
{
	exchange_t* exg = exg_unpack(data);
}
