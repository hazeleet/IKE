#include <stdlib.h>
#include "exchange.h"
#include "log.h"

exchange_t* exg_create()
{
	exchange_t* this = calloc(1, sizeof(exchange_t));
	this->header.length = IKE_HEADER_LENGTH;

	return this;
}

chunk_t* exg_pack(exchange_t* exg)
{
	chunk_t* packed = chk_create();
	chk_write(packed, &exg->header.SPIi, 8);
	chk_write(packed, &exg->header.SPIr, 8);
	chk_write(packed, &exg->header.next_payload, 1);
	chk_write(packed, &exg->header.version, 1);
	chk_write(packed, &exg->header.exchange_type, 1);
	chk_write(packed, &exg->header.flags, 1);
	chk_rwrite(packed, &exg->header.message_id, 4);
	chk_rwrite(packed, &exg->header.length, 4);

	if(exg->payloads) {
		chunk_t* pld_packed = pld_pack(exg->payloads);
		chk_write_chk(packed, pld_packed);
		chk_free(pld_packed);
	}

	return packed;
}

exchange_t* exg_unpack(chunk_t* packed)
{
	exchange_t* exg = exg_create();
	chk_read(packed, &exg->header.SPIi, 8);
	chk_read(packed, &exg->header.SPIr, 8);
	chk_read(packed, &exg->header.next_payload, 1);
	chk_read(packed, &exg->header.version, 1);
	chk_read(packed, &exg->header.exchange_type, 1);
	chk_read(packed, &exg->header.flags, 1);
	chk_rread(packed, &exg->header.message_id, 4);
	chk_rread(packed, &exg->header.length, 4);

	logging(DBG, "[EXG] unpacking\n");
	logging(DBG, "      SPIi: 0x%llX\n", exg->header.SPIi);
	logging(DBG, "      SPIr: 0x%llX\n", exg->header.SPIr);
	logging(DBG, "      next pld: %d\n", exg->header.next_payload);
	logging(DBG, "      version: %d\n", exg->header.version);
	logging(DBG, "      type: %d\n", exg->header.exchange_type);
	logging(DBG, "      flags: 0x%X\n", exg->header.flags);
	logging(DBG, "      message id: %d\n", exg->header.message_id);
	logging(DBG, "      length: %d\n", exg->header.length);
	if(exg->header.next_payload != PLD_NO) {
		exg->payloads = pld_unpack(packed, exg->header.next_payload);
	}

	return exg;
}

void exg_push(exchange_t* exg, payload_t* pld)
{
	pld->header.next_payload = exg->header.next_payload;
	pld->next = exg->payloads;

	exg->header.next_payload = pld->type;
	exg->header.length += pld->header.length;
	exg->payloads = pld;
}
