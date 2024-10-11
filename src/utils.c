#include "utils.h"
#include <stdlib.h>
#include <time.h>

chunk_t* get_rand(int bytes)
{
	chunk_t* chk = chk_create();
	unsigned char* buf = malloc(bytes);

	srand(time(NULL)+(*buf));
	for(int i = 0; i < bytes; i++)
		buf[i] = rand();

	chk_write(chk, buf, bytes);
	free(buf);

	return chk;
}
