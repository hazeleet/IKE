#include "payload.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

payload_t* pld_create(ike_payload_type type)
{
	payload_t* this = calloc(1, sizeof(payload_t));
	this->type = type;
	this->header.length = IKE_PAYLOAD_HEADER_LENGTH;

	return this;
}

chunk_t* pld_pack(payload_t* pld)
{
	chunk_t* packed = chk_create();

	chk_write(packed, &pld->header.next_payload, 1);
	chk_write(packed, &pld->header.reserved, 1);
	chk_write(packed, &pld->header.length, 2);

	switch(pld->type) {
		case PLD_Nx:
			chk_write(packed, pld->body.Nx.data, pld->header.length - IKE_PAYLOAD_HEADER_LENGTH);
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
	chk_read(packed, &pld->header.length, 2);
	logging(DBG, "[PLD] %d payload unpacking\n", type);
	logging(DBG, "      next pld: %d\n", pld->header.next_payload);
	logging(DBG, "      length: %d\n", pld->header.length);

	switch(type) {
		case PLD_Nx:
			{
				int nonce_size = pld->header.length - IKE_PAYLOAD_HEADER_LENGTH;
				pld->body.Nx.data = calloc(1, nonce_size);
				chk_read(packed, pld->body.Nx.data, nonce_size);
				logging(DBG, "      nonce: \n");
				logging_hex(DBG, pld->body.Nx.data, nonce_size);
			}
			break;
	}
	if(pld->header.next_payload != PLD_NO) {
		pld->next = pld_unpack(packed, pld->header.next_payload);
	}

	return pld;
}

void pld_nx_set(payload_t* pld, chunk_t* nonce)
{
	pld->header.length += nonce->size;
	pld->body.Nx.data = calloc(1, nonce->size);
	memcpy(pld->body.Nx.data, nonce->ptr, nonce->size);
}
