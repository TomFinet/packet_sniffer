#ifndef CS241_LINKEDLIST_H
#define CS241_LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

struct set {
    struct linkedlist *ll;
    size_t size;
};

struct set *create_set();

int add(struct set *s, unsigned char *data, size_t len);
int contains_element(struct set *s, unsigned char *data, size_t len);

struct node *first(struct set *s);
size_t size(struct set *s);
void print_set(struct set *s);

#endif