#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "builtins.h"

#include "forth.h"

Forth forth_new(Code c) {
    Forth f;
    if((f = malloc(sizeof(*f))) == NULL) { exit(1); }
    #define X(r) f->registers[r] = 0;
    
    REGISTERS
    
    #undef X
    f->data = dstack_new(50);
    f->call = cstack_new(50);
    f->code = c;
    f->ip = block_getInstrSeq(code_getBlock(c,0));
    f->cb = 0;
    f->zombie = 0;
    return f;
}

void forth_free(Forth f) {
    dstack_free(f->data);
    cstack_free(f->call);
    code_free(f->code);
    free(f);
}

int forth_running(Forth f) {
    return f->zombie == 0;
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
    assert (f->zombie == 0);
    instr_print(*f->ip);
    forth_exec(f,*f->ip);
    printStacks(f);
    //printf("data: %d, call: %d\n",dstack_size(f->data),cstack_size(f->call));
}

void forth_call(Forth f, codesize_t sub) {
    assert(sub >= 0 && sub < code_getSize(f->code));
    if(!cstack_isFull(f->call)) {
        cstack_push(f->call, seq_next(f->ip));
        f->ip = block_getInstrSeq(code_getBlock(f->code,sub));
        f->cb = sub;
    } else {
        forth_exit(f);
    }
}

void forth_exit(Forth f) {
    if(!dstack_isEmpty(f->data)) {
        printf("result: %li\n",dstack_pop(f->data));
    } else {
        puts("nothing");
    }
    f->zombie = 1;
}
