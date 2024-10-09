#include "payload_nx.h"
#include <stdlib.h>
#include <time.h>

payload_nx_t* pld_nx_create(int nonce_size)
{
	payload_nx_t* pld = calloc(1, sizeof(payload_nx_t));
	pld->nonce = chk_create();

	srand(time(NULL));
	unsigned char* buf = malloc(nonce_size);
	for(int i = 0; i < nonce_size; i++)
		buf[i] = rand();

	chk_write(pld->nonce, buf, nonce_size);
	free(buf);

	return pld;
}
