#ifndef CS241_DISPATCH_H
#define CS241_DISPATCH_H

#include <signal.h>
#include <pcap.h>
#include <pthread.h>
#include <unistd.h>

#include "packet.h"
#include "analysis.h"
#include "queue.h"

#define NUM_THREADS 5

extern pcap_t *pcap_handle;

pthread_t tid[NUM_THREADS];

struct queue *work_queue;

static volatile int run = 1;

void init();
void teardown();

void dispatch(struct packet *pkt);

void sig_handler(int signal);
void *runner(void *arg);

#endif
