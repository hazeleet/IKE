#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <libconfig.h>
#include "netype.h"

typedef struct cfg_peer_t cfg_peer_t;
struct cfg_peer_t {
  struct {
    ip4_addr addr;
  }left, right;

  cfg_peer_t* next;
};

typedef struct {
  cfg_peer_t* peers;
}configure_t;

configure_t* cfg_create(const char* filename);


#endif //__CONFIG_H__
