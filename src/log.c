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

void logging_hex(log_level_t lev, chunk_t* chk)
{
	// width is fixed size; 16
	char hbuf[49] = {'\0',};	// hex buffer
	char cbuf[33] = {'\0',};	// char buffer

	// i is chunks's index
	// j is hex&char buf's index
	for(int i = 0, j = 0; i < chk->size; i++, j++) {
		unsigned char c = 0xFF & ((char*)chk->ptr)[i];

		// hex
		sprintf(&hbuf[j*3], "%02X ", c);

		// character
		if(isprint(c))
			sprintf(&cbuf[j*2], " %c", c);
		else
			sprintf(&cbuf[j*2], " .");

		// end of width OR chunk
		if(j == 15 || i+1 == chk->size) {
			j++;
			hbuf[j*3] = cbuf[j*2] = '\0';
			j = -1;
			logging(lev, "      %-48s %s\n", hbuf, cbuf);
		}
	}
}
