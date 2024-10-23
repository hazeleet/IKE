#include "log.h"
#include "daemon.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

log_t* log_create()
{
	log_t* log = calloc(1, sizeof(log_t));
	log->level = ALL;

	return log;
}

void log_free(log_t* log)
{
	free(log);
}

void logging(log_level_t lev, const char* fmt, ...)
{
	if(IKE.log->level <= lev) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}
}

void logging_hex(log_level_t lev, void* data, int size)
{
	// width is fixed size; 16
	char hbuf[49] = {'\0',};	// hex buffer
	char cbuf[33] = {'\0',};	// char buffer

	// i is chunks's index
	// j is hex&char buf's index
	for(int i = 0, j = 0; i < size; i++, j++) {
		unsigned char c = 0xFF & ((char*)data)[i];

		// hex
		sprintf(&hbuf[j*3], "%02X ", c);

		// character
		if(isprint(c))
			sprintf(&cbuf[j*2], " %c", c);
		else
			sprintf(&cbuf[j*2], " .");

		// end of width OR chunk
		if(j == 15 || i+1 == size) {
			j++;
			hbuf[j*3] = cbuf[j*2] = '\0';
			j = -1;
			logging(lev, "      %-48s %s\n", hbuf, cbuf);
		}
	}
}

void logging_chk(log_level_t lev, chunk_t* chk)
{
	logging_hex(lev, chk->ptr, chk->size);
}

char* log_exchange_type(ike_exchange_type type)
{
	switch (type) {
		case IKE_SA_INIT: return "IKE SA INIT";
		case IKE_AUTH: return "IKE AUTH";
		case CREATE_CHILD_SA: return "CREATE CHILD SA";
		case INFORMATIONAL: return "INFORMATIONAL";
	}
}

char*	log_payload_type(ike_payload_type type)
{
	switch (type){
		case PLD_NO: return "No";
		case PLD_SA: return "Security Association";
		case PLD_KE: return "Key Exchange";
		case PLD_IDi: return "Identification - Initiator";
		case PLD_IDr: return "Identification - Responder";
		case PLD_CERT: return "Certificate";
		case PLD_CERTREQ: return "Certificate Request";
		case PLD_AUTH: return "Authentication";
		case PLD_Nx: return "Nonce";
		case PLD_N: return "Notify";
		case PLD_D: return "Delete";
		case PLD_V: return "Vendor ID";
		case PLD_TSi: return "Traffic Selector - Initiator";
		case PLD_TSr: return "Traffic Selector - Responder";
		case PLD_SK: return "Encrypted and Authenticated";
		case PLD_CP: return "Configuration";
		case PLD_EAP: return "Extensible Authentication";
	}
}
