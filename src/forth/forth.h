#ifndef FORTH_H
#define FORTH_H

#include "instruction.h"
#include "code.h"
#include "cstack.h"
#include "dstack.h"
#include "cell.h"

typedef struct Forth {
    Number registers[10];
    DStack data;
    CStack call;
    Code code;
    Cell cell;
    Instruction *ip;
    codesize_t cb;
    int zombie;
} *Forth;

Forth forth_new(Code c);

void forth_free(Forth f);

int forth_running(Forth f);

void forth_exec(Forth f, Instruction instr);

void forth_step(Forth f);

void forth_call(Forth f, codesize_t sub);

void forth_exit(Forth f);


#endif /* FORTH_H */
