#include "daemon.h"

#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

static char doc[] = "IKE";
static char args_doc[] = "";

static struct argp_option options[] = {
	{"daemon", 'd', 0, 0, "Enable daemon mode" , 0},
	{"conf_file", 'c', "FILE", 0, "Config file" , 0},
	{0}
};

static error_t parse_opt(int key, char* arg, struct argp_state *state)
{
	daemon_args_t *arguments = state->input;

	switch(key) {
		case 'd':
			arguments->daemon_mode = 1;
			break;
		case 'c':
			arguments->conf_file = arg;
			break;
		case ARGP_KEY_ARG:
		case ARGP_KEY_END:
			return 0;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL};

int main(int argc, char** argv) {
	daemon_args_t arguments;

	arguments.daemon_mode = 0;
	arguments.conf_file = "conf/sample.conf";

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	daemon_create(arguments);

	net_running();
	daemon_running();
	while(1);

	return 0;
}
