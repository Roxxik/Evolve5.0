#ifndef FORTH_H
#define FORTH_H

#include "instruction.h"
#include "code.h"
#include "cstack.h"
#include "dstack.h"

typedef struct Forth {
    DStack data;
    CStack call;
    Code code;
    Instruction *ip;
    int zombie;
} *Forth;

Forth forth_new(Code c);

void forth_free(Forth f);

int forth_running(Forth f);

void forth_step(Forth f);

void forth_exit(Forth f);


#endif /* FORTH_H */
