#include "chunk.h"

#include <stdlib.h>
#include <string.h>

chunk_t* chk_create()
{
	chunk_t* chk = calloc(1, sizeof(chunk_t));
	chk->ptr = calloc(0, 0);

	return chk;
}

void chk_free(chunk_t* chk)
{
	free(chk->ptr);
	free(chk);
}

void _chk_write(chunk_t* chk, void* data, int size, bool reverse)
{
	if(size > 0) {
		int new_size = chk->size + size;
		if(chk->offset < size)
			chk->ptr = realloc(chk->ptr, chk->size + (size - chk->offset));

		if(chk->offset != 0) {
			memmove(chk->ptr, chk->ptr + chk->offset, chk->size);
			chk->offset = 0;
		}

		char* d = chk->ptr + chk->offset + chk->size;
		if(reverse) {
			char* s = data + size - 1;
			for(int i = 0; i < size; i++) {
				*d = *s;
				d++;
				s--;
			}
		} else {
			memcpy(d, data, size);
		}

		chk->size = new_size;
	}
}

void chk_write_chk(chunk_t* chk, chunk_t* src)
{
	_chk_write(chk, src->ptr, src->size, false);
}

void _chk_read(chunk_t* chk, void* dest, int size, bool reverse)
{
	if(size > 0) {
		size = (size > chk->size)? chk->size: size;

		char* d = dest;
		if(reverse) {
			char* s = chk->ptr + chk->offset + size - 1;
			for(int i = 0; i < size; i++) {
				*d = *s;
				d++;
				s--;
			}
		} else {
			memcpy(d, chk->ptr + chk->offset, size);
		}

		chk->offset = chk->offset + size;
		chk->size = chk->size - size;
	}
}

void chk_read_chk(chunk_t* chk, chunk_t* dest, int size)
{
	void* tmp = calloc(1, size);
	_chk_read(chk, tmp, size, false);
	_chk_write(dest, tmp, size, false);
	free(tmp);
}
