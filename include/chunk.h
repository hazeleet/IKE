#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <stdbool.h>

typedef struct {
  void*   ptr;
  int     size;
  int     offset;
}chunk_t;

chunk_t*  chk_create();
void      chk_free(chunk_t* chk);

void      _chk_write(chunk_t* chk, void* data, int size, bool reverse);
#define   chk_write(chk, data, size) _chk_write(chk, data, size, false)
#define   chk_rwrite(chk, data, size) _chk_write(chk, data, size, true)
void      chk_write_chk(chunk_t* chk, chunk_t* src);

void      _chk_read(chunk_t* chk, void* dest, int size, bool reverse);
#define   chk_read(chk, dest, size) _chk_read(chk, dest, size, false)
#define   chk_rread(chk, dest,  size) _chk_read(chk, dest, size, true)
void      chk_read_chk(chunk_t* chk, chunk_t* dest, int size);

#endif //__CHUNK_H__
