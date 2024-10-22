#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

#include "ike_protocol.h"
#include "chunk.h"

typedef struct payload_t payload_t;
struct payload_t {
  ike_payload_header_t   header;
  union {
    ike_payload_nx_t  Nx;
    ike_payload_ke_t  KE;
		struct {
			proposal_t*				proposals;
		}SA;
  };

  ike_payload_type    type;
  payload_t*          next;
};

payload_t*    pld_create(ike_payload_type type);
chunk_t*      pld_pack(payload_t* pld);
payload_t*    pld_unpack(chunk_t* packed, ike_payload_type type);

// Nx
void          pld_nx_set(payload_t* pld, chunk_t* nonce);

// KE
void          pld_ke_set(payload_t* pld, ike_dh_id dh, chunk_t* key);

#endif //__PAYLOAD_H__
