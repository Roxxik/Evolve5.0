#ifndef DSTACK_H
#define DSTACK_H

#include <stdint.h>

#include "instruction.h"

typedef uint16_t dstacksize_t;

typedef struct DStack{
    dstacksize_t maxSize;
    dstacksize_t size;
    Number *stack;
} *DStack;


void dstack_print(DStack s);

DStack dstack_new(dstacksize_t initialsize);
void dstack_free(DStack s);

int dstack_isEmpty(DStack s);
int dstack_isFull(DStack s);
dstacksize_t dstack_size(DStack s);
dstacksize_t dstack_maxSize(DStack s);

void dstack_push(DStack s, Number n);
Number dstack_peek(DStack s);
Number dstack_pop(DStack s);


#endif /* CSTACK_H */
