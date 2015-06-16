#include "forth.h"
#include "cstack.h"
#include "dstack.h"

#include "builtins.h"

void instrNOOP(Forth f) {
    f->ip = seq_next(f->ip);
}

void instrEXIT(Forth f) {
    forth_exit(f);
    /*if (!cstack_isEmpty(f->call)) {
        f->ip = cstack_pop(f->call);
    } else {
        forth_exit(f);
    }*/
}

void instrNUM(Forth f) {
    if (!dstack_isFull(f->data)) {
        dstack_push(f->data,*(Number*)(f->ip+1));
        f->ip = seq_next(f->ip);
    } else {
        forth_exit(f);
    }
}

void instrADD(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n1 = dstack_pop(f->data);
        Number n2 = dstack_pop(f->data);
        dstack_push(f->data, n1+n2);
    }
    f->ip = seq_next(f->ip);
}
