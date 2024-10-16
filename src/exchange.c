#include <stdlib.h>
#include "exchange.h"

exchange_t* exg_create()
{
	exchange_t* this = calloc(1, sizeof(exchange_t));
	this->header.length = 28;

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

	// payload pack

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

	return exg;
}
