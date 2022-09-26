#ifndef CS241_LINKEDLIST_H
#define CS241_LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    unsigned char *data;
    struct node *next;
};

struct linkedlist {
    struct node *head;
    struct node *tail;
};

struct linkedlist *create_linkedlist();

int isempty(struct linkedlist *ll);
int contains(struct linkedlist *ll, unsigned char *data, size_t len);

void insert(struct linkedlist *ll, unsigned char *data, size_t len);
unsigned char *deleteHead(struct linkedlist *ll);

struct node *head(struct linkedlist *ll);
void print_linkedlist(struct linkedlist *ll);

void free_linkedlist(struct linkedlist *ll);

#endif