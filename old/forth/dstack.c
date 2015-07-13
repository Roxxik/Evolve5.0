#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "util.h"

#include "dstack.h"

void dstack_print(DStack s) {
    for(int size = dstack_size(s) - 1; size >= 0; size--) {
        printf("d: %li\n",s->stack[size]);
    }
}

DStack dstack_new(dstacksize_t initialsize) {
    DStack s;
    MALLOC(s, sizeof(*s));
    s->maxSize = initialsize;
    s->size = 0;
    MALLOC(s->stack, initialsize * sizeof(Number));
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

Number dstack_peek(DStack s) {
    assert(!dstack_isEmpty(s));
    return s->stack[s->size-1];
}

Number dstack_pop(DStack s) {
    assert (!dstack_isEmpty(s));
    return s->stack[--s->size];
}
