#include <stdlib.h>
#include "exchange.h"

exchange_t* exg_create()
{
	exchange_t* this = calloc(1, sizeof(exchange_t));

	return this;
}

chunk_t* exg_pack(exchange_t* exg)
{
	chunk_t* packed = chk_create();

	return packed;
}
