#ifndef CS241_QUEUE_H
#define CS241_QUEUE_H

#include <stdlib.h>

#include "linkedlist.h"

struct queue {
    struct linkedlist *ll;
};

struct queue *create_queue();

void enqueue(struct queue *q, unsigned char *data, size_t len);
unsigned char *dequeue(struct queue *q);

#endif