#include <assert.h>
#include <math.h>

#include "forth.h"
#include "cstack.h"
#include "dstack.h"

#include "builtins.h"

//keeping it safe, not an actual instruction,
//just defined to determine the number of instructions

void instrINSTRUCTION_MAX(Forth f __attribute__((unused))) {
    assert(0);
}

void instrNOOP(Forth f) {
    f->ip = seq_next(f->ip);
}
//reflection
void instrEXEC(Forth f) {
    if (dstack_size(f->data) >= 1) {
        Number instr = dstack_pop(f->data);
        if (instr >= 0 && instr < INSTR_INSTRUCTION_MAX) {
            forth_exec(f,instr);
        }
    }
    f->ip = seq_next(f->ip);
}

//Flow control
void instrEXIT(Forth f) {
    if (cstack_size(f->call) >= 1) {
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
    dstack_push(f->data, f->cb);
}

//number
void instrNUM(Forth f) {
    dstack_push(f->data,*(Number*)(f->ip+1));
    f->ip = seq_next(f->ip);
}

//stack
void instrDUP(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number dup = dstack_peek(f->data);
        dstack_push(f->data, dup);
    }
    f->ip = seq_next(f->ip);
}

void instrPOP(Forth f) {
    if(dstack_size(f->data) >= 1) {
        dstack_pop(f->data);
    }
    f->ip = seq_next(f->ip);
}

void instrSWAP(Forth f) {
    if(dstack_size(f->data) >= 2) {
        Number b = dstack_pop(f->data);
        Number a = dstack_pop(f->data);
        dstack_push(f->data,b);
        dstack_push(f->data,a);
    }
    f->ip = seq_next(f->ip);
}

void instrOVER(Forth f) {
    if(dstack_size(f->data) >= 2) {
        Number b = dstack_pop(f->data);
        Number a = dstack_peek(f->data);
        dstack_push(f->data, b);
        dstack_push(f->data, a);
    }
    f->ip = seq_next(f->ip);
}

void instrROT(Forth f) {
    if(dstack_size(f->data) >= 3) {
        Number c = dstack_pop(f->data);
        Number b = dstack_pop(f->data);
        Number a = dstack_pop(f->data);
        dstack_push(f->data, b);
        dstack_push(f->data, c);
        dstack_push(f->data, a);
    }
    f->ip = seq_next(f->ip);
}

/*
void instrPICK(Forth f) {
//won't implement this for once    
}*/

void instrCONDDUP(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number dup = dstack_peek(f->data);
        if(dup) {
            dstack_push(f->data, dup);
        }
    }
    f->ip = seq_next(f->ip);
}

void instrTOR(Forth f) {//ROT backwards
    if(dstack_size(f->data) >= 3) {
        Number c = dstack_pop(f->data);
        Number b = dstack_pop(f->data);
        Number a = dstack_pop(f->data);
        dstack_push(f->data, c);
        dstack_push(f->data, a);
        dstack_push(f->data, b);
    }
    f->ip = seq_next(f->ip);
}


//arithmetics

//unary
void instrINC(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n + 1);
    }
    f->ip = seq_next(f->ip);
}

void instrDEC(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n - 1);
    }
    f->ip = seq_next(f->ip);
}

void instrINC2(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n + 2);
    }
    f->ip = seq_next(f->ip);
}

void instrDEC2(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n - 2);
    }
    f->ip = seq_next(f->ip);
}

void instrHALF(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n / 2);
    }
    f->ip = seq_next(f->ip);
}

void instrDOUBLE(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n * 2);
    }
    f->ip = seq_next(f->ip);
}

void instrABS(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n > 0 ? n : -n);
    }
    f->ip = seq_next(f->ip);
}

void instrSQRT(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        if(n >= 0) {
            dstack_push(f->data, sqrt(n));
        }
    }
    f->ip = seq_next(f->ip);
}

void instrZERO(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, n == 0);
    }
    f->ip = seq_next(f->ip);
}

void instrNEGATE(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, -n);
    }
    f->ip = seq_next(f->ip);
}

void instrNOT(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, !n);
    }
    f->ip = seq_next(f->ip);
}

void instrINVERT(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, ~n);
    }
    f->ip = seq_next(f->ip);
}

void instrSGN(Forth f) {
    if(dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        dstack_push(f->data, (n>0) - (n<0));
    }
    f->ip = seq_next(f->ip);
}

//binary
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

void instrDIVMOD(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        if(n2 != 0) {
            dstack_push(f->data, n1 % n2);
            dstack_push(f->data, n1 / n2);
        }
    }
    f->ip = seq_next(f->ip);
}

void instrEQ(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 == n2);
    }
    f->ip = seq_next(f->ip);
}

void instrNE(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 != n2);
    }
    f->ip = seq_next(f->ip);
}

void instrLT(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 < n2);
    }
    f->ip = seq_next(f->ip);
}

void instrGT(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 > n2);
    }
    f->ip = seq_next(f->ip);
}

void instrLE(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 <= n2);
    }
    f->ip = seq_next(f->ip);
}

void instrGE(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 >= n2);
    }
    f->ip = seq_next(f->ip);
}

void instrOR(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 | n2);
    }
    f->ip = seq_next(f->ip);
}

void instrAND(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 & n2);
    }
    f->ip = seq_next(f->ip);
}

void instrXOR(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 ^ n2);
    }
    f->ip = seq_next(f->ip);
}

void instrMIN(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 < n2 ? n1 : n2);
    }
    f->ip = seq_next(f->ip);
}

void instrMAX(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        dstack_push(f->data, n1 > n2 ? n1 : n2);
    }
    f->ip = seq_next(f->ip);
}

//packing
void instrPACK2(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        union {
            struct { Number n; };
            struct { int32_t a, b; };
        } p = { .a = (int32_t)n1, .b = (int32_t)n2 };
        dstack_push(f->data, p.n);
    }
    f->ip = seq_next(f->ip);
}

void instrUNPACK2(Forth f) {
    if (dstack_size(f->data) >= 1) {
        union {
            struct { Number n; };
            struct { int32_t a, b; };
        } p = { .n = dstack_pop(f->data) };
        dstack_push(f->data, p.a);
        dstack_push(f->data, p.b);
    }
    f->ip = seq_next(f->ip);
}

void instrPACK4(Forth f) {
    if (dstack_size(f->data) >= 4) {
        Number n4 = dstack_pop(f->data);
        Number n3 = dstack_pop(f->data);
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        union {
            struct { Number n; };
            struct { int16_t a, b, c, d; };
        } p = { .a = n1, .b = n2, .c = n3, .d = n4 };
        dstack_push(f->data, p.n);
    }
    f->ip = seq_next(f->ip);
}

void instrUNPACK4(Forth f) {
    if (dstack_size(f->data) >= 1) {
        union {
            struct { Number n; };
            struct { int16_t a, b, c, d; };
        } p = { .n = dstack_pop(f->data) };
        dstack_push(f->data, p.a);
        dstack_push(f->data, p.b);
        dstack_push(f->data, p.c);
        dstack_push(f->data, p.d);
    }
    f->ip = seq_next(f->ip);
}

//registers
void instrRN(Forth f) {
    if (dstack_size(f->data) >= 1) {
        Number n = dstack_pop(f->data);
        if(n < REGISTER_MAX) {
            dstack_push(f->data,f->registers[n]);
        }
    }
    f->ip = seq_next(f->ip);
}

#define X(r) \
void instrR##r(Forth f) {\
    dstack_push(f->data,f->registers[r]);\
    f->ip = seq_next(f->ip);\
}

REGISTERS

#undef X

void instrSETRN(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n = dstack_pop(f->data);
        if(n < REGISTER_MAX) {
            f->registers[n] = dstack_pop(f->data);;
        }
    }
    f->ip = seq_next(f->ip);
}

#define X(r) \
void instrSETR##r(Forth f) {\
    if(dstack_size(f->data) >= 1) {\
        f->registers[r] = dstack_pop(f->data);\
    }\
    f->ip = seq_next(f->ip);\
}

REGISTERS

#undef X
