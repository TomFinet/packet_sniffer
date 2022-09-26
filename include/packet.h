#ifndef CS241_PACKET_H
#define CS241_PACKET_H

struct packet {
    unsigned char *data;
    unsigned int len;
    int verbose;
};

#endif
