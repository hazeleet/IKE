#include "payload.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

ike_proposal_t* _pld_sa_set_proposal(proposal_t* proposal, int number);
ike_proposal_t*	_pld_sa_unpack(chunk_t* packed);

payload_t* pld_create(ike_payload_type type)
{
	payload_t* this = calloc(1, sizeof(payload_t));
	this->type = type;

	return this;
}

chunk_t* pld_pack(payload_t* pld)
{
	chunk_t* packed = chk_create();

	chk_write(packed, &pld->header.next_payload, 1);
	chk_write(packed, &pld->header.reserved, 1);
	chk_rwrite(packed, &pld->header.length, 2);
	logging(DBG, "[PLD] %s payload packing (%d-bytes)\n", log_payload_type(pld->type), pld->header.length);

	switch(pld->type) {
		case PLD_Nx:
			chk_write(packed, pld->Nx.data, pld->header.length - IKE_PAYLOAD_HEADER_LENGTH);
			break;
		case PLD_KE:
			chk_rwrite(packed, &pld->KE.dh_group, 2);
			chk_rwrite(packed, &pld->KE.reserved, 2);
			chk_write(packed, pld->KE.data,
					pld->header.length - IKE_PAYLOAD_KE_FIXED_LENGTH);
			break;
		case PLD_SA:
			{
				for(ike_proposal_t* proposal = pld->SA.proposals;
						proposal != NULL; proposal = proposal->next) {
					chk_write(packed, &proposal->last, 1);
					chk_write(packed, &proposal->reserved, 1);
					chk_rwrite(packed, &proposal->length, 2);
					chk_write(packed, &proposal->number, 1);
					chk_write(packed, &proposal->protocol, 1);
					chk_write(packed, &proposal->spi_size, 1);
					chk_write(packed, &proposal->num_of_transforms, 1);
					//spi
					ike_transform_t* transforms = proposal->transforms;
					for(int i = 0; i < proposal->num_of_transforms; i++) {
						chk_write(packed, &transforms[i].last, 1);
						chk_write(packed, &transforms[i].reserved1, 1);
						chk_rwrite(packed, &transforms[i].length, 2);
						chk_write(packed, &transforms[i].type, 1);
						chk_write(packed, &transforms[i].reserved2, 1);
						chk_rwrite(packed, &transforms[i].id, 2);
						// attribute
					}
				}
			}
			break;
	}

	if(pld->next) {
		chunk_t* tmp = pld_pack(pld->next);
		chk_write_chk(packed, tmp);
		chk_free(tmp);
	}

	return packed;
}

payload_t* pld_unpack(chunk_t* packed, ike_payload_type type)
{
	payload_t* pld = pld_create(type);
	chk_read(packed, &pld->header.next_payload, 1);
	chk_read(packed, &pld->header.reserved, 1);
	chk_rread(packed, &pld->header.length, 2);
	logging(DBG, "[PLD] %s payload unpacking\n", log_payload_type(pld->type));
	logging(DBG, "      next pld: %s\n", log_payload_type(pld->header.next_payload));
	logging(DBG, "      length: %d\n", pld->header.length);

	switch(type) {
		case PLD_Nx:
			{
				int nonce_size = pld->header.length - IKE_PAYLOAD_HEADER_LENGTH;
				pld->Nx.data = calloc(1, nonce_size);
				chk_read(packed, pld->Nx.data, nonce_size);
				logging(DBG, "      nonce: \n");
				logging_hex(DBG, pld->Nx.data, nonce_size);
			}
			break;
		case PLD_KE:
			{
				int key_size = pld->header.length - IKE_PAYLOAD_KE_FIXED_LENGTH;
				chk_rread(packed, &pld->KE.dh_group, 2);
				chk_rread(packed, &pld->KE.reserved, 2);
				pld->KE.data = calloc(1, key_size);
				chk_read(packed, pld->KE.data, key_size);
				logging(DBG, "      dh: %d\n", pld->KE.dh_group);
				logging(DBG, "      key: \n");
				logging_hex(DBG, pld->KE.data, key_size);
			}
			break;
		case PLD_SA:
			{
				pld->SA.proposals = _pld_sa_unpack(packed);
			}
			break;
	}
	if(pld->header.next_payload != PLD_NO) {
		pld->next = pld_unpack(packed, pld->header.next_payload);
	}

	return pld;
}

ike_proposal_t*	_pld_sa_unpack(chunk_t* packed)
{
	ike_proposal_t* this = calloc(1, sizeof(ike_proposal_t));
	chk_read(packed, &this->last, 1);
	chk_read(packed, &this->reserved, 1);
	chk_rread(packed, &this->length, 2);
	chk_read(packed, &this->number, 1);
	chk_read(packed, &this->protocol, 1);
	chk_read(packed, &this->spi_size, 1);
	chk_read(packed, &this->num_of_transforms, 1);
	// spi
	logging(DBG, "      proposal[%d]\n", this->number);
	logging(DBG, "        last: %d\n", this->last);
	logging(DBG, "        reserved: %d\n", this->reserved);
	logging(DBG, "        length: %d\n", this->length);
	logging(DBG, "        number: %d\n", this->number);
	logging(DBG, "        ptotocol: %d\n", this->protocol);
	logging(DBG, "        spi_size: %d\n", this->spi_size);
	logging(DBG, "        num_of_transforms: %d\n", this->num_of_transforms);

	// transforms
	if(this->num_of_transforms > 0) {
		this->transforms = calloc(this->num_of_transforms, sizeof(ike_transform_t));
		ike_transform_t* transforms = this->transforms;
		for(int i = 0; i < this->num_of_transforms; i++) {
			chk_read(packed, &transforms[i].last, 1);
			chk_read(packed, &transforms[i].reserved1, 1);
			chk_rread(packed, &transforms[i].length, 2);
			chk_read(packed, &transforms[i].type, 1);
			chk_read(packed, &transforms[i].reserved2, 1);
			chk_rread(packed, &transforms[i].id, 2);
			// attributes

			logging(DBG, "        transform[%d]\n", i+1);
			logging(DBG, "          last: %d\n", transforms[i].last);
			logging(DBG, "          reserved1: %d\n", transforms[i].reserved1);
			logging(DBG, "          length: %d\n", transforms[i].length);
			logging(DBG, "          type: %d\n", transforms[i].type);
			logging(DBG, "          reserved2: %d\n", transforms[i].reserved2);
			logging(DBG, "          id: %d\n", transforms[i].id);
		}
	}

	if(this->last != IKE_PROPOSAL_LAST)
		this->next = _pld_sa_unpack(packed);

	return this;
}

void pld_nx_set(payload_t* this, chunk_t* nonce)
{
	this->header.length = IKE_PAYLOAD_Nx_FIXED_LENGTH + nonce->size;
	this->Nx.data = calloc(1, nonce->size);
	memcpy(this->Nx.data, nonce->ptr, nonce->size);
}

chunk_t* pld_nx_get(payload_t* this)
{
	chunk_t* nonce = chk_create();
	chk_write(nonce, this->Nx.data,
			this->header.length - IKE_PAYLOAD_HEADER_LENGTH);
	return nonce;
}

void pld_ke_set(payload_t* this, ike_dh_id dh, chunk_t* key)
{
	this->header.length = IKE_PAYLOAD_KE_FIXED_LENGTH + key->size;
	this->KE.dh_group = dh;
	this->KE.data = calloc(1, key->size);
	memcpy(this->KE.data, key->ptr, key->size);
}

chunk_t* pld_ke_get_key(payload_t* this)
{
	chunk_t* key = chk_create();
	chk_write(key, this->KE.data,
			this->header.length - IKE_PAYLOAD_KE_FIXED_LENGTH);
	return key;
}

ike_proposal_t* _pld_sa_set_proposal(proposal_t* proposal, int number)
{
	ike_proposal_t* this = NULL;
	if(proposal) {
		this = calloc(1, sizeof(ike_proposal_t));
		this->next = _pld_sa_set_proposal(proposal->next, number+1);
		this->last = (this->next)? IKE_PROPOSAL_MORE: IKE_PROPOSAL_LAST;
		this->length = IKE_PROPOSAL_FIXED_LENGTH;
		this->number = number;
		this->protocol = proposal->protocol;
		//cur->spi_size;
		//cur->spi;
		this->num_of_transforms = proposal->num_of_transforms;

		// trannsforms
		if(this->num_of_transforms > 0) {
			this->transforms = calloc(this->num_of_transforms, sizeof(ike_transform_t));
			ike_transform_t* transforms = this->transforms;
			int i;
			for(i = 0; i < this->num_of_transforms; i++) {
				transforms[i].last = IKE_TRANSFORM_MORE;
				transforms[i].length = IKE_TRANSFORM_FIXED_LENGTH;
				transforms[i].type = proposal->transforms[i].type;
				transforms[i].id = proposal->transforms[i].id;

				this->length += transforms[i].length;
			}
			transforms[i-1].last = IKE_TRANSFORM_LAST;
		}
	}
	return this;
}

void pld_sa_set(payload_t* this, proposal_t* proposal)
{
	this->SA.proposals = _pld_sa_set_proposal(proposal, 1);
	this->header.length = IKE_PAYLOAD_SA_FIXED_LENGTH
		+ ((ike_proposal_t*)this->SA.proposals)->length;
}

proposal_t* pld_sa_get(payload_t* this)
{
}
