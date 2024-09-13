#include "network.h"
#include "log.h"

#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

void*				_receiving(void* arg);
void*				_sending(void* arg);

network_t* net_create()
{
	network_t* net = calloc(1, sizeof(network_t));
	net->q_send = que_create();
	net->q_recv = que_create();

	net->port = htons(500);

	net->sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(net->sock < 0) {
		logging(ERR, "[NET] Failure create socket\n");
		return NULL;
	}
	int opt = 1;
	setsockopt(net->sock, IPPROTO_IP, IP_PKTINFO, &opt, sizeof(opt));
	logging(ALL, "[NET] Create Socket\n");

	return net;
}

void net_free(network_t* net)
{
	packet_t* d;
	for(d = dequeue(net->q_send); d != NULL; d = dequeue(net->q_send))
		pkt_free(d);
	for(d = dequeue(net->q_recv); d != NULL; d = dequeue(net->q_recv))
		pkt_free(d);

	free(net->q_send);
	free(net->q_recv);
	free(net);
}

void net_send(network_t* net, packet_t* pkt)
{
	enqueue(net->q_send, pkt);
}

packet_t* net_recv(network_t* net)
{
	return dequeue(net->q_recv);
}

void* _receiving(void* arg)
{
	network_t* net = arg;
	struct sockaddr_in addr, client;
	int recv_len;
	char buf[1024];
	struct msghdr msg;
	struct iovec iov;
	char ancillary[64];
	struct cmsghdr *cm;
	char src[IPSTR_LEN], dst[IPSTR_LEN];
	struct in_pktinfo *pktinfo;

	addr.sin_family = AF_INET;
	addr.sin_port = net->port;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(net->sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		logging(ERR, "[NET] Failure bind socket\n");
		return NULL;
	}

	msg.msg_name = &client;
	msg.msg_namelen = sizeof(client);
	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = ancillary;
	msg.msg_controllen = sizeof(ancillary);
	msg.msg_flags = 0;

	while(1) {
		recv_len = recvmsg(net->sock, &msg, 0);
		if(recv_len) {
			for(cm = CMSG_FIRSTHDR(&msg); cm != NULL;
					cm = CMSG_NXTHDR(&msg, cm)) {
				if(cm->cmsg_level == IPPROTO_IP) {
					pktinfo = (struct in_pktinfo*)CMSG_DATA(cm);

					packet_t* pkt = pkt_create();
					pkt->data = chk_create();
					chk_write(pkt->data, buf, recv_len);
					pkt->src = client.sin_addr.s_addr;
					pkt->dst = pktinfo->ipi_addr.s_addr;

					net_atos(pkt->src, src, IPSTR_LEN);
					net_atos(pkt->dst, dst, IPSTR_LEN);
					logging(DBG, "[NET] Receive %d-byte packet(%s->%s)\n",
							recv_len, src, dst);
					enqueue(net->q_recv, pkt);
					break;
				}
			}
		}
	}
}

void* _sending(void* arg)
{
	network_t* net = arg;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = net->port;

	while(1) {
		packet_t* pkt = dequeue(net->q_send);
		addr.sin_addr.s_addr = pkt->dst;

		sendto(net->sock, pkt->data->ptr, pkt->data->size, 0, (struct sockaddr*)&addr, sizeof(addr));
		logging(DBG, "[NET] Send %d-byte packet\n", pkt->data->size);
		pkt_free(pkt);
	}
}

void net_running(network_t* net)
{
	pthread_t tid;
	pthread_create(&tid, NULL, _sending, net);
	logging(ALL, "[NET] Start Sending\n");
	pthread_create(&tid, NULL, _receiving, net);
	logging(ALL, "[NET] Start Receiving\n");
}

