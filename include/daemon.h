#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "network.h"
#include "log.h"
#include "config.h"
#include "samanager.h"

typedef struct {
  log_t*        log;
  network_t*    net;
  configure_t*  cfg;
  samanager_t*  sam;
}daemon_t;

extern daemon_t IKE;

void daemon_create();

void daemon_running();

#endif //__DAEMON_H__
