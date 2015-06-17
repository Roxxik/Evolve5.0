#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#include "cstack.h"


void cstack_print(CStack s) {
    for(int size = cstack_size(s) - 1; size >= 0; size--) {
        printf("c: %p\n",s->stack[size]);
    }
}

CStack cstack_new(cstacksize_t initialsize) {
    CStack s;
    if((s = malloc(sizeof(*s))) == NULL) { exit(1); }
    s->maxSize = initialsize;
    s->size = 0;
    if((s->stack = malloc(initialsize * sizeof(Instruction*))) == NULL) { exit(1); }
    return s;
}

void cstack_free(CStack s) {
    free(s->stack);
    free(s);
}

int cstack_isEmpty(CStack s) {
    return cstack_size(s) == 0;
}

int cstack_isFull(CStack s) {
    return cstack_size(s) == cstack_maxSize(s);
}

cstacksize_t cstack_size(CStack s) {
    return s->size;
}

cstacksize_t cstack_maxSize(CStack s) {
    return s->maxSize;
}

void cstack_push(CStack s, Instruction *i) {
    assert (!cstack_isFull(s));
    s->stack[s->size++] = i;
}

Instruction *cstack_pop(CStack s) {
    assert (!cstack_isEmpty(s));
    return s->stack[--s->size];
}
