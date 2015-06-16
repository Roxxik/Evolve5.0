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
        Number sub = dstack_pop(f->data);
        if(sub >= 0 && sub < code_getSize(f->code)) {
            forth_call(f,sub);
            return;
        }
    }
    f->ip = seq_next(f->ip);
}

void instrIF(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number sub = dstack_pop(f->data);
        Number expr = dstack_pop(f->data);
        if(expr && sub >= 0 && sub < code_getSize(f->code)) {
            forth_call(f,sub);
            return;
        }
    }
    f->ip = seq_next(f->ip);    
}

void instrIFELSE(Forth f) {
    if (dstack_size(f->data) >= 3) {
        Number elseSub = dstack_pop(f->data);
        Number ifSub = dstack_pop(f->data);
        Number expr = dstack_pop(f->data);
        
        Number sub = expr ? ifSub : elseSub;
        if(sub >= 0 && sub < code_getSize(f->code)) {
            forth_call(f,sub);
            return;
        }
    }
    f->ip = seq_next(f->ip);
}

void instrLOOP(Forth f) {
    if (dstack_size(f->data) >= 1
    && dstack_pop(f->data)) {
        f->ip = block_getInstrSeq(code_getBlock(f->code,f->cb));
    } else {
        f->ip = seq_next(f->ip);
    }
}

void instrCONDEXIT(Forth f) {
    if (dstack_size(f->data) >= 1
    && dstack_pop(f->data)) {
        instrEXIT(f);
    } else {
        f->ip = seq_next(f->ip);
    }
}

void instrCB(Forth f) {
    if (!dstack_isFull(f->data)) {
        dstack_push(f->data, f->cb);
    } else {
        forth_exit(f);
    }
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
            dstack_push(f->data, dup);
        }
    }
    f->ip = seq_next(f->ip);
}

void instrPOP(Forth f) {
    if(dstack_size(f->data) >= 1) {
        dstack_pop(f->data);
    }
    f->ip = seq_next(f->ip);
}

//arithmetics
void instrADD(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 + n2);
    }
    f->ip = seq_next(f->ip);
}

void instrSUB(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 - n2);
    }
    f->ip = seq_next(f->ip);
}

void instrMUL(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 * n2);
    }
    f->ip = seq_next(f->ip);
}

void instrDIV(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        if(n2 != 0) {
            dstack_push(f->data, n1 / n2);
        }
    }
    f->ip = seq_next(f->ip);
}

void instrMOD(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        if(n2 != 0) {
            dstack_push(f->data, n1 % n2);
        }
    }
    f->ip = seq_next(f->ip);
}
