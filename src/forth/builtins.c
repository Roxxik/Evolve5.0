#include "forth.h"
#include "cstack.h"
#include "dstack.h"

#include "builtins.h"

void instrNOOP(Forth f) {
    f->ip = seq_next(f->ip);
}

//Flow control
void instrEXIT(Forth f) {
    if (!cstack_isEmpty(f->call)) {
        f->ip = cstack_pop(f->call);
    } else {
        forth_exit(f);
    }
}

void instrCALL(Forth f) {
    if (dstack_size(f->data) >= 1) {
        Number subRoutine = dstack_pop(f->data);
        if(subRoutine >= 0 && subRoutine < code_getSize(f->code)) {
            if(!cstack_isFull(f->call)) {
                cstack_push(f->call,seq_next(f->ip));
                f->ip = block_getInstrSeq(code_getBlock(f->code,subRoutine));
            } else {
                //forth_expandCall(f);
                forth_exit(f);
            }
            return;
        }
    }
    f->ip = seq_next(f->ip);
}

//number
void instrNUM(Forth f) {
    if (!dstack_isFull(f->data)) {
        dstack_push(f->data,*(Number*)(f->ip+1));
        f->ip = seq_next(f->ip);
    } else {
        //forth_expandData(f);
        forth_exit(f);
    }
}

//stack
void instrDUP(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number dup = dstack_pop(f->data);
        dstack_push(f->data,dup);
        if(!dstack_isFull(f->data)) {
            dstack_push(f->data,dup);
        }
    }
    f->ip = seq_next(f->ip);
}

//arithmetics
void instrADD(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n1 = dstack_pop(f->data);
        Number n2 = dstack_pop(f->data);
        dstack_push(f->data, n1+n2);
    }
    f->ip = seq_next(f->ip);
}
