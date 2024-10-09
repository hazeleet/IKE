#include "payload_nx.h"
#include "utils.h"
#include <stdlib.h>

payload_nx_t* pld_nx_create(int nonce_size)
{
	payload_nx_t* pld = calloc(1, sizeof(payload_nx_t));
	pld->nonce = get_rand(nonce_size);

	return pld;
}
