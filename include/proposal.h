#ifndef __PROPOSAL_H__
#define __PROPOSAL_H__

#define TRANSFORM_MAX 10

#include "ike_protocol.h"

typedef struct proposal_t proposal_t;
struct proposal_t {
	ike_protocol_id		protocol;

	struct transform_t {
		struct transform_body_t {
			int id;
			int key_size;
		}body[TRANSFORM_MAX];
		int size;
	}transforms[6];

	proposal_t*		next;
};

proposal_t*		ppl_create(ike_protocol_id protocol);
void					ppl_push(proposal_t* this, proposal_t* ppl);

void					_ppl_set(proposal_t* this, ike_transform_type type, int id, int key_size);
#define				ppl_set_encr(this, id, key_size) _ppl_set(this, IKE_ENCR, id, key_size)
#define				ppl_set_prf(this, id) _ppl_set(this, IKE_PRF, id, 0)
#define				ppl_set_integ(this, id) _ppl_set(this, IKE_INTEG, id, 0)
#define				ppl_set_dh(this, id) _ppl_set(this, IKE_DH, id, 0)
#define				ppl_set_esn(this, id) _ppl_set(this, IKE_ESN, id, 0)

#endif //__PROPOSAL_H__
