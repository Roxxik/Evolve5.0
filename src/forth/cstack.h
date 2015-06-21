#ifndef CSTACK_H
#define CSTACK_H

#include <stdint.h>

#include "instruction.h"

typedef uint16_t cstacksize_t;

typedef struct call_t {
    Instruction *ip;
    Number cb;
} call_t;

typedef struct CStack{
    cstacksize_t maxSize;
    cstacksize_t size;
    call_t *stack;
} *CStack;


void cstack_print(CStack s);

CStack cstack_new(cstacksize_t initialsize);
void cstack_free(CStack s);

int cstack_isEmpty(CStack s);
int cstack_isFull(CStack s);
cstacksize_t cstack_size(CStack s);
cstacksize_t cstack_maxSize(CStack s);

void cstack_push(CStack s, Instruction *i, Number cb);
call_t cstack_pop(CStack s);


#endif /* CSTACK_H */
