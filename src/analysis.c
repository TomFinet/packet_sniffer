#include "analysis.h"

pthread_mutex_t arp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t syn_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t http_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Initialises the variables and data structures used to 
 * store analysis results.
 */
void initstats() {
    ip_set = create_set();
    url_blacklist = create_set();
    char *google_url = "www.google.co.uk";
    add(url_blacklist, google_url, sizeof(google_url));

    arp_packets = 0;
    syn_packets = 0;
    blacklist_vio = 0;
    num_uniq_ips = 0;
}

/**
 * Calls function to handle the link-layer packet analysis.
 * 
 * pkt: structure containing packet length and data.
 */
void analyse(struct packet *pkt) {
    parse_ether(pkt);
}

/**
 * Parses an ethernet packet header.
 * 
 * pkt: structure containing packet length and data.
 */
void parse_ether(struct packet *pkt) {
    struct ether_header *eth_hdr = (struct ether_header *) pkt->data;

    unsigned short ether_type = ntohs(eth_hdr->ether_type);

    pkt->data = pkt->data + ETH_HLEN;
    pkt->len = pkt->len - ETH_HLEN;

    if(ether_type == ETHERTYPE_ARP) { // ARP packet
        parse_arp(pkt);
    } else if(ether_type == ETHERTYPE_IP) { // IP packet
        parse_ip(pkt);
    }
}

/**
 * Parses an ARP packet header.
 * 
 * pkt: structure containing packet length and data.
 */
void parse_arp(struct packet *pkt) {
    struct arphdr *arp_hdr = (struct arphdr *) pkt->data;
    unsigned int arpop = ntohs(arp_hdr->ar_op);
    if(arpop == ARPOP_REPLY) {
        pthread_mutex_lock(&arp_mutex);
        arp_packets++;
        pthread_mutex_unlock(&arp_mutex);
    }
}

/**
 * Parses an IP packet header.
 * 
 * pkt: structure containing packet length and data.
 */
void parse_ip(struct packet *pkt) {
    struct iphdr *ip_hdr = (struct iphdr *) pkt->data;

    unsigned int hdr_len = ip_hdr->ihl * 4;

    if(ip_hdr->protocol == TCP_PROTOCOL_NUM) {
        pkt->data = pkt->data + hdr_len;
        pkt->len = pkt->len - hdr_len;

        parse_tcp(pkt, ip_hdr->saddr);
    }
}

/**
 * Parses a TCP packet header.
 * 
 * pkt: structure containing packet length and data.
 * src_ip: the IP address of the packet source.
 */
void parse_tcp(struct packet *pkt, unsigned int src_ip) {
    struct tcphdr *tcp_hdr = (struct tcphdr *) pkt->data;

    unsigned int hdr_len = tcp_hdr->doff * 4;
    unsigned int dest = ntohs(tcp_hdr->dest);

    int flsum = tcp_hdr->urg + tcp_hdr->ack + tcp_hdr->psh + tcp_hdr->rst + tcp_hdr->fin;
    if(flsum == 0 && tcp_hdr->syn) {
      pthread_mutex_lock(&syn_mutex);
      syn_packets++;
      int success = add(ip_set, (unsigned char *) &src_ip, IPV4_BYTES);
      num_uniq_ips += success;
      pthread_mutex_unlock(&syn_mutex);
    } else if(dest == HTTP_PORT) { // http packet
        pkt->data = pkt->data + hdr_len;
        pkt->len = pkt->len - hdr_len;
        parse_http(pkt);
    }
}

/**
 * Checks the contents of an HTTP request for any blacklisted urls.
 * 
 * pkt: structure containing packet length and data.
 */
void parse_http(struct packet *pkt) {
    struct node *curr = first(url_blacklist);
    char *url;
    int i;
    for(i = 0; i < size(url_blacklist); i++) {
        url = curr->data;
        curr = curr->next;
        char *found = strstr(pkt->data, url);
        if(found) {
            pthread_mutex_lock(&http_mutex);
            blacklist_vio++;
            pthread_mutex_unlock(&http_mutex);
        }
    }
}

/**
 * Frees memory used by data structures used to store analysis results.
 */
void clear_mem() {
    free(ip_set);
    free(url_blacklist);
}
