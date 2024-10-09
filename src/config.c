#include "config.h"
#include "log.h"

#include <stdlib.h>

cfg_peer_t*   _cfg_create_peer();

cfg_peer_t* _cfg_create_peer()
{
	cfg_peer_t* peer = calloc(1, sizeof(cfg_peer_t));

	return peer;
}

void _parsing(configure_t* cfg, const char* filename)
{
	config_t cf;
	config_setting_t *left, *right;
	const char* buf;

	config_init(&cf);

	if(!config_read_file(&cf, filename)) {
		logging(ERR, "[CFG] Failure init config(%s:%d - %s)\n", config_error_file(&cf), config_error_line(&cf), config_error_text(&cf));
		config_destroy(&cf);
		return;
	}

	config_setting_t* peers = config_lookup(&cf, "peers");
	if(peers != NULL) {
		int count = config_setting_length(peers);
		cfg_peer_t* last_peer = cfg->peers;

		for(int i = 0; i < count; i++) {
			config_setting_t* cf_p = config_setting_get_elem(peers, i);
			cfg_peer_t* peer = _cfg_create_peer();

			left = config_setting_lookup(cf_p, "left");
			if(left != NULL) {
				if(config_setting_lookup_string(left, "ip", &buf))
					peer->left.addr = net_stoa(buf);
			}

			right = config_setting_lookup(cf_p, "right");
			if(right != NULL) {
				if(config_setting_lookup_string(right, "ip", &buf))
					peer->right.addr = net_stoa(buf);
			}

			if(last_peer != NULL) {
				last_peer->next = peer;
			} else {
				cfg->peers = peer;
			}
			last_peer = peer;

			// LOGGING
			char src[IPSTR_LEN], dst[IPSTR_LEN];
			net_atos(peer->left.addr, src, IPSTR_LEN);
			net_atos(peer->right.addr, dst, IPSTR_LEN);
			logging(DBG, "[CFG] Find Peer%d (%s->%s)\n",
					i, src, dst);
		}
	}

}

configure_t* cfg_create(const char* filename)
{
	configure_t* cfg = calloc(1, sizeof(configure_t));

	_parsing(cfg, filename);

	return cfg;
}
