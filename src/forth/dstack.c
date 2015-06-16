#include <stdlib.h>
#include <assert.h>


#include "dstack.h"

DStack dstack_new(dstacksize_t initialsize) {
    DStack s;
    if((s = malloc(sizeof(*s))) == NULL) { exit(1); }
    s->maxSize = initialsize;
    s->size = 0;
    if((s->stack = malloc(initialsize * sizeof(Number))) == NULL) { exit(1); }
    return s;
}

void dstack_free(DStack s) {
    free(s->stack);
    free(s);
}

int dstack_isEmpty(DStack s) {
    return dstack_size(s) == 0;
}

int dstack_isFull(DStack s) {
    return dstack_size(s) == dstack_maxSize(s);
}

dstacksize_t dstack_size(DStack s) {
    return s->size;
}

dstacksize_t dstack_maxSize(DStack s) {
    return s->maxSize;
}

void dstack_push(DStack s, Number n) {
    assert (!dstack_isFull(s));
    s->stack[s->size++] = n;
}

Number dstack_pop(DStack s) {
    assert (!dstack_isEmpty(s));
    return s->stack[--s->size];
}
