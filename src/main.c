#include <stdio.h>

#include "daemon.h"

#include <stdlib.h>

int main() {
	daemon_create();

	net_running(IKE.net);
	daemon_running();
	while(1);

	return 0;
}
