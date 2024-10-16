#include "payload.h"
#include <stdlib.h>

payload_t* pld_create(ike_payload_type type)
{
	payload_t* this = calloc(1, sizeof(payload_t));
	this->type = type;

	return this;
}
