#include "set.h"

struct set *create_set(void) {
    struct set *s = (struct set *) malloc(sizeof(struct set));
    s->ll = create_linkedlist();
    return s;
}

int add(struct set *s, unsigned char *data, size_t len) {
    if(contains(s->ll, data, len)) return 0;
    insert(s->ll, data, len);
    s->size++;
    return 1;
}

int contains_element(struct set *s, unsigned char *data, size_t len) {
    contains(s->ll, data, len);
}

struct node *first(struct set *s) {
    return head(s->ll);
}

size_t size(struct set *s) {
    return s->size;
}

void print_set(struct set *s) {
    print_linkedlist(s->ll);
}