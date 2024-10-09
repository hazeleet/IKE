#include "payload.h"
#include <stdlib.h>

payload_t* pld_create(PAYLOAD_TYPE type)
{
	payload_t* pld = calloc(1, sizeof(payload_t));

	switch(type) {
		case Nx:
			break;
	}

	return pld;
}
