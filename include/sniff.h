#ifndef CS241_SNIFF_H
#define CS241_SNIFF_H

#include <pcap.h>

#include "packet.h" 
#include "dispatch.h"

pcap_t *pcap_handle;

extern unsigned int arp_packets;
extern unsigned int syn_packets;
extern unsigned int blacklist_vio;
extern unsigned int num_uniq_ips;

void sniff(char *interface, int verbose);

void *packet_handler(unsigned char *args, 
                    const struct pcap_pkthdr *hInfo,
                    const unsigned char *packet);

void print_report();
void dump(const unsigned char *data, int length);

#endif
