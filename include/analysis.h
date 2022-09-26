#ifndef CS241_ANALYSIS_H
#define CS241_ANALYSIS_H

#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <pcap.h>

#include <net/ethernet.h>
#include <net/if_arp.h>

#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include "linkedlist.h"
#include "set.h"
#include "packet.h"

#define TCP_PROTOCOL_NUM 0x06
#define HTTP_PORT 80
#define IPV4_BYTES 4

struct set *url_blacklist;
struct set *ip_set;

unsigned int arp_packets;
unsigned int syn_packets;
unsigned int blacklist_vio;
unsigned int num_uniq_ips;

void initstats();

void analyse(struct packet *pkt);

void parse_ether(struct packet *pkt);
void parse_arp(struct packet *pkt);
void parse_ip(struct packet *pkt);
void parse_tcp(struct packet *pkt, unsigned int src_ip);
void parse_http(struct packet *pkt);

void clear_mem();

#endif
