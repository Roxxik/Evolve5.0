#include <limits.h>
#include <assert.h>
#include <math.h>

#include "forth.h"
#include "cstack.h"
#include "dstack.h"
#include "env.h"

#include "builtins.h"

#define NUMBER_BIT (Number)sizeof(Number) * CHAR_BIT

Number rotr(Number value, Number shift);

//pre: shift > 0
Number rotl(Number value, Number shift) {
    if(shift < 0) {
        return rotr(value, -shift);
    }
    shift %= sizeof(value) * CHAR_BIT;
    return (value << shift) | (value >> (sizeof(value) * CHAR_BIT - shift));
}

//pre: shift > 0
Number rotr(Number value, Number shift) {
    if(shift < 0) {
        return rotl(value, -shift);
    }
    shift %= sizeof(value) * CHAR_BIT;
    return (value >> shift) | (value << (sizeof(value) * CHAR_BIT - shift));
}

//logical negative shifts put in 1s instead of 0s
//ignoring the arithmetical shift for now
Number shiftl(Number value, Number shift) {
    Number zero = 0;
    if(shift >= 0) {
        if(shift < NUMBER_BIT) {
            return value << shift;
        } else {
            return zero;
        }
    } else {
        shift = -shift;
        if(shift < NUMBER_BIT) {
            return ~((~value) << shift);
        } else {
            return ~zero;
        }
    }
}

Number shiftr(Number value, Number shift) {
    Number zero = 0;
    union {
        struct { Number i; };
        struct { UNumber u; };
    } v = { .i = value };
    if(shift >= 0) {
        if(shift < NUMBER_BIT) {
            v.u >>= shift;
            return v.i;
        } else {
            return zero;
        }
    } else {
        shift = -shift;
        if(shift < NUMBER_BIT) {
            v.u = ~((~v.u) >> shift);
            return v.i;
        } else {
            return ~zero;
        }
    }    
}

#define UNARYOPS \
    X(INC,    n + 1)\
    X(DEC,    n - 1)\
    X(INCTWO,   n + 2)\
    X(DECTWO,   n - 2)\
    X(HALF,   n / 2)\
    X(DOUBLE, n * 2)\
    X(ABS,    n > 0 ? n : -n)\
    X(ZERO,   n == 0)\
    X(NEGATE, -n)\
    X(NOT,    !n)\
    X(INVERT, ~n)\
    X(SGN,    (n>0) - (n<0))\
    X(SQUARE, n * n)\
    X(CUBE,   n * n * n)

#define UNARYOPS_NONNEG \
    X(SQRT,   sqrt(n))\
    X(CBRT,   cbrt(n))

#define BINARYOPS \
    X(ADD,  n1 +  n2)\
    X(SUB,  n1 -  n2)\
    X(MUL,  n1 *  n2)\
    X(EQ,   n1 == n2)\
    X(NE,   n1 != n2)\
    X(LT,   n1 <  n2)\
    X(GT,   n1 >  n2)\
    X(LE,   n1 <= n2)\
    X(GE,   n1 >= n2)\
    X(OR,   n1 |  n2)\
    X(AND,  n1 &  n2)\
    X(XOR,  n1 ^  n2)\
    X(LAND, n1 && n2)\
    X(LOR,  n1 || n2)\
    X(MIN,  n1 < n2 ? n1 : n2)\
    X(MAX,  n1 > n2 ? n1 : n2)\
    X(SHL,  shiftl(n1, n2))\
    X(SHR,  shiftr(n1, n2))\
    X(RTL,  rotl(n1, n2))\
    X(RTR,  rotr(n1, n2))
    
#define BINARYOPS_N2_NOTZERO \
    X(DIV, n1 / n2)\
    X(MOD, n1 % n2)


//keeping it safe, not an actual instruction,
//just defined to determine the number of instructions

void instrINSTRUCTION_MAX(Forth f __attribute__((unused))) {
    f->ip = seq_next(f->ip);
}

void instrNOOP(Forth f) {
    f->ip = seq_next(f->ip);
}
//reflection
void instrEXEC(Forth f) {
    if (dstack_size(f->data) >= 1) {
        Number instr = dstack_pop(f->data);
        if(instr >= 0 && instr < INSTR_INSTRUCTION_MAX && instr != INSTR_NUM) {
            forth_exec(f,instr);
        } else {
            f->ip = seq_next(f->ip);
        }
    }
}

//Flow control
void instrEXIT(Forth f) {
    if (cstack_size(f->call) >= 1) {
        call_t call = cstack_pop(f->call);
        f->ip = call.ip;
        f->cb = call.cb;
    } else {
        //kill this thing if it finished main
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

//push number
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

void instrDUP2(Forth f) {
    if(dstack_size(f->data) >= 2) {
        Number d2 = dstack_pop(f->data);
        Number d1 = dstack_peek(f->data);
        dstack_push(f->data, d2);
        dstack_push(f->data, d1);
        dstack_push(f->data, d2);
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
#define X(name,op) \
void instr##name(Forth f) {\
    if(dstack_size(f->data) >= 1) {\
        Number n = dstack_pop(f->data);\
        dstack_push(f->data, op);\
    }\
    f->ip = seq_next(f->ip);\
}\

UNARYOPS

#undef X

#define X(name,op) \
void instr##name(Forth f) {\
    if(dstack_size(f->data) >= 1) {\
        Number n = dstack_pop(f->data);\
        if(n >= 0) {\
            dstack_push(f->data, op);\
        }\
    }\
    f->ip = seq_next(f->ip);\
}\

UNARYOPS_NONNEG

#undef X

//binary
#define X(name,op) \
void instr##name(Forth f) {\
    if (dstack_size(f->data) >= 2) {\
        Number n2 = dstack_pop(f->data);\
        Number n1 = dstack_pop(f->data);\
        dstack_push(f->data, op);\
    }\
    f->ip = seq_next(f->ip);\
}\

BINARYOPS

#undef X

#define X(name,op) \
void instr##name(Forth f) {\
    if (dstack_size(f->data) >= 2) {\
        Number n2 = dstack_pop(f->data);\
        Number n1 = dstack_pop(f->data);\
        if(n2 != 0) {\
            dstack_push(f->data, op);\
        }\
    }\
    f->ip = seq_next(f->ip);\
}\

BINARYOPS_N2_NOTZERO

#undef X

//divmod has a special place, cause it pushes two things back
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


//packing
void instrPACK2(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number n2 = dstack_pop(f->data);
        Number n1 = dstack_pop(f->data);
        union {
            struct { Number n; };
            struct { int32_t a, b; };
        } p = { .a = n1, .b = n2 };
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
        if(n >= 0 && n < REGISTER_MAX) {
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
        if(n >= 0 && n < REGISTER_MAX) {
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

//environmental stuff
void instrMOVE(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number y = dstack_pop(f->data);
        Number x = dstack_pop(f->data);
        env_move(f,x,y);
    }
    f->ip = seq_next(f->ip);
}

void instrEAT(Forth f) {
    if (dstack_size(f->data) >= 2) {
        Number y = dstack_pop(f->data);
        Number x = dstack_pop(f->data);
        env_eat(f,x,y);
    }
    f->ip = seq_next(f->ip);
}

void instrSEED(Forth f) {
    if (dstack_size(f->data) >= 3) {
        Number y = dstack_pop(f->data);
        Number x = dstack_pop(f->data);
        Number nrg = dstack_pop(f->data);
        env_seed(f,x,y,nrg);
    }
    f->ip = seq_next(f->ip);
}
