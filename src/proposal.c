#include "proposal.h"
#include <stdlib.h>

proposal_t*	ppl_create(ike_protocol_id protocol)
{
	proposal_t* this = calloc(1, sizeof(proposal_t));
	this->protocol = protocol;

	return this;
}

void ppl_push(proposal_t* this, proposal_t* ppl)
{
	proposal_t* last;
	for(last = this; last->next != NULL; last = last->next);
	last->next = ppl;
}

void _ppl_set(proposal_t* this, ike_transform_type type, int id, int key_size)
{
	if(this->num_of_transforms < TRANSFORM_MAX) {
		this->transforms[this->num_of_transforms].type = type;
		this->transforms[this->num_of_transforms].id = id;
		this->transforms[this->num_of_transforms].key_size = key_size;
		this->num_of_transforms++;
	}
	else {
		// WTF
	}
}
