#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "ike_protocol.h"
#include "chunk.h"

typedef struct {
  struct {
    ike_dh_id   group;
    chunk_t*    key;
  }DH;

	struct {
		ike_encr_id		id;
		int						key_length;
	}ENCR;
}crypto_t;

#endif //__CRYPTO_H__
