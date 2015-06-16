#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "builtins.h"

#include "forth.h"

Forth forth_new(Code c) {
    Forth f;
    if((f = malloc(sizeof(*f))) == NULL) { exit(1); }
    f->data = dstack_new(50);
    f->call = cstack_new(50);
    f->code = c;
    f->ip = block_getInstrSeq(code_getBlock(c,0));
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

void forth_step (Forth f) {
    assert (f->zombie == 0);
    instr_print(*f->ip);
    switch (*f->ip) {
        #define X(name) case INSTR_##name: instr##name(f);break;
        
        INSTRUCTIONS
        
        #undef X
        default: instrNOOP(f);break;
    }
}

void forth_exit(Forth f) {
    printf("result: %li\n",dstack_pop(f->data));
    f->zombie = 1;
}
