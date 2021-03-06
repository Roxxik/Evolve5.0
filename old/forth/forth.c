#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "builtins.h"
#include "env.h"
#include "util.h"

#include "forth.h"

Forth forth_new(Code c, identifier_t parentID, generation_t parentGen, step_t step, coord_t x, coord_t y, energy_t nrg) {
    Forth f;
    MALLOC(f, sizeof(*f));

    f->id = env_newID();
    f->parentID = parentID;
    f->generation = parentGen + 1;
    f->step = step;
    f->x = x;
    f->y = y;
    f->energy = nrg;
    f->used = 0;

    #define X(r) f->registers[r] = 0;
    
    REGISTERS
    
    #undef X
    
    f->data = dstack_new(64);
    f->call = cstack_new(64);
    f->code = c;
    f->ip = block_getInstrSeq(code_getBlock(c,0));
    f->cb = 0;
    f->zombie = false;
    return f;
}

void forth_free(Forth f) {
    dstack_free(f->data);
    cstack_free(f->call);
    code_free(f->code);
    free(f);
}

bool forth_running(Forth f) {
    return !f->zombie;
}

void forth_exec(Forth f, Instruction instr) {
    switch (instr) {
        #define X(name) case INSTR_##name: instr##name(f);break;
        
        INSTRUCTIONS
        
        #undef X
        default: instrNOOP(f);break;
    }
}

void printStacks(Forth f) {
    dstack_print(f->data);
    cstack_print(f->call);
}

void forth_step (Forth f) {
    assert (!f->zombie);
    //instr_print(*f->ip);
    forth_exec(f,*f->ip);
    //printStacks(f);
    //printf("data: %d, call: %d\n",dstack_size(f->data),cstack_size(f->call));
    if(f->energy == 0 || cstack_size(f->call) >= 50 || dstack_size(f->data) >= 50) {
        forth_exit(f);
    }
}

void forth_call(Forth f, codesize_t sub) {
    assert(sub >= 0 && sub < code_getSize(f->code));
    if(!cstack_isFull(f->call)) {
        cstack_push(f->call, seq_next(f->ip), f->cb);
        f->ip = block_getInstrSeq(code_getBlock(f->code,sub));
        f->cb = sub;
    } else {
        forth_exit(f);
    }
}

Forth forth_seed(Forth f, coord_t x, coord_t y, energy_t nrg) {
    if(nrg >= f->energy) {
        nrg = f->energy;
    }
    f->energy -= nrg;
    return forth_new(code_mutate(f->code),f->id,f->generation,0,x,y,nrg);
}

void forth_exit(Forth f) {
    f->zombie = true;
}
