#include "sniff.h"

/**
 * Opens a network interface to sniff packets from.
 * Calls functions to handle graceful end of program execution
 * as well as printing the final analysis report.
 * 
 * interface: the interface packets are being read from.
 * verbose: mode deciding how verbose output is.
 * 
 */
void sniff(char *interface, int verbose) {

    // create threads
    init();

    // Open network interface for packet capture
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_handle = pcap_open_live(interface, 4096, 1, 0, errbuf);
    if (pcap_handle == NULL) {
        fprintf(stderr, "Unable to open interface %s\n", errbuf);
        exit(EXIT_FAILURE);
    } else {
        printf("SUCCESS! Opened %s for capture\n", interface);
    }

    unsigned char *arg = (unsigned char *) &verbose;
    pcap_loop(pcap_handle, -1, packet_handler, arg);
    pcap_close(pcap_handle);
    
    teardown();
    print_report();
}

/**
 * Called every time a new packet is received on the interface being listened to.
 * Passes each packet to dispatch to be added to a queue.
 * 
 * args: user defined function arguments.
 * hinfo: packet information (length of packet, timestamp, etc)
 * packet: actual packet data.
 */
void *packet_handler(unsigned char *args,
                    const struct pcap_pkthdr *hinfo,
                    const unsigned char *packet) {

    struct packet *pkt = (struct packet *) malloc(sizeof(struct packet)); 
    pkt->data = packet;
    pkt->len = hinfo->len;
    pkt->verbose = (int) *args;
    dispatch(pkt);

    return NULL;
}

/**
 * Prints packet analysis report.
 */
void print_report() {
    printf("\nIntrusion detection report\n");
    printf("%d SYN packets detected from %d different IPs\n", syn_packets, num_uniq_ips);
    printf("%d ARP responses\n", arp_packets);
    printf("%d URL blacklist violations\n", blacklist_vio);
}
