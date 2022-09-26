#include "queue.h"

struct queue *create_queue(void) {
    struct queue *q = (struct queue *) malloc(sizeof(struct queue));
    q->ll = create_linkedlist();
    return q;
}

void enqueue(struct queue *q, unsigned char *data, size_t len) {
    insert(q->ll, data, len);
}

unsigned char *dequeue(struct queue *q) {
    return deleteHead(q->ll);
}