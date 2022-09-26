#include "linkedlist.h"

struct linkedlist *create_linkedlist() {
    struct linkedlist *ll = (struct linkedlist *) malloc(sizeof(struct linkedlist));
    ll->head = NULL;
    ll->tail = NULL;
    return ll;
}

struct node *head(struct linkedlist *ll) {
    return ll->head;
}

int isempty(struct linkedlist *ll) {
    return ll->head == NULL;
}

int contains(struct linkedlist *ll, unsigned char *data, size_t len) {
    struct node *curr = ll->head;
    while(curr != NULL) {
        if(memcmp(curr->data, data, len) == 0) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void insert(struct linkedlist *ll, unsigned char *data, size_t len) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    new_node->data = (unsigned char *) malloc(len * sizeof(unsigned char));
    memmove((void *) new_node->data, (void *) data, len);
    
    new_node->next = NULL;
    if(isempty(ll)) {
        ll->head = new_node;
        ll->tail = new_node;
    } else {
        ll->tail->next = new_node;
        ll->tail = new_node;
    }
}

unsigned char *deleteHead(struct linkedlist *ll) {
    struct node *head_node;
    if(isempty(ll)) {
        return NULL;
    } else {
        head_node = ll->head;
        ll->head = ll->head->next;
        if(ll->head == NULL) {
            ll->tail = NULL;
        }
        return head_node->data;
    }
}

void print_linkedlist(struct linkedlist *ll) {
    struct node *curr = ll->head;
    while(curr != NULL) {
        printf("%d\n", curr->data);
        curr = curr->next;
    }
}

void free_linkedlist(struct linkedlist *ll) {
    free(ll);
}