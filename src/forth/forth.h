#ifndef FORTH_H
#define FORTH_H

#include <stdbool.h>

#include "instruction.h"
#include "code.h"
#include "cstack.h"
#include "dstack.h"

typedef uint32_t identifier_t;
typedef uint16_t generation_t;
typedef uint32_t step_t;
typedef uint16_t coord_t;
typedef int8_t offset_t; /* [-1 .. 1] */
typedef uint32_t energy_t;


typedef struct Forth {
    identifier_t id;
    identifier_t parentID;
    generation_t generation;
    step_t step;
    coord_t x,y;
    energy_t energy;
    energy_t used;

    Number registers[10];
    DStack data;
    CStack call;
    Code code;
    Instruction *ip;
    codesize_t cb;
    bool zombie;
} *Forth;



Forth forth_new(Code c, identifier_t parentID, generation_t parentGen, step_t step, coord_t x, coord_t y, energy_t nrg);

void forth_free(Forth f);

bool forth_running(Forth f);

void forth_exec(Forth f, Instruction instr);

void forth_step(Forth f);

void forth_call(Forth f, codesize_t sub);

void forth_exit(Forth f);


#endif /* FORTH_H */
