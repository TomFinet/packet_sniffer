#include "dispatch.h"

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

/**
 * Called once to create the threads, the worker 
 * queue and set up a signal handler.
 */
void init() {
    signal(SIGINT, sig_handler);

    work_queue = create_queue();
    initstats();

    int i;
    for(i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, runner, NULL);
    }
}

/**
 * Frees memory (threads, worker queue, data structures 
 * used for analysis) before program ends.
 */
void teardown() {
    // unblock all threads
    pthread_cond_broadcast(&queue_cond);
    int i;
    for(i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    clear_mem();
    free(work_queue);
}

/**
 * Adds each incoming packet to the work queue.
 * Notifies a waiting thread that a new packet has entered the queue.
 * 
 * pkt: structure containing packet length and data.
 */
void dispatch(struct packet *pkt) {
    // Add new packet to the work queue
    pthread_mutex_lock(&queue_mutex);
    enqueue(work_queue, (unsigned char *) pkt, pkt->len);
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
}

/**
 * Handles SIGINT signal by stopping the listening of
 * new packets on the chosen interface and toggling
 * the run flag to 0.
 */
void sig_handler(int signal) {
    pcap_breakloop(pcap_handle);
    run = 0;
}

/**
 * Function called when a new thread is created. Handles
 * reading from the work queue and waiting when it is empty.
 * Calls analyse if a packet is dequeued from the work queue.
 */
void *runner(void *arg) {
    while(run || !isempty(work_queue->ll)) {

        pthread_mutex_lock(&queue_mutex);
        while(run && isempty(work_queue->ll)) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        struct packet *pkt = (struct packet *) dequeue(work_queue);
        pthread_mutex_unlock(&queue_mutex);

        if(pkt != NULL) {
            analyse(pkt);
        }
    }

    return NULL;
}
