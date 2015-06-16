#ifndef INSTRUCTION_H
#define INSTRUCTION_H
/*
 * all instruction sequences can be sequencial in memory too
 * the number instruction interpretes the next 8 Bytes as signed 64-Bit int
 * all instruction sequences MUST end with an exit instruction
 */
#include <stdint.h>

#define INSTRUCTIONS \
    X(NOOP)\
    X(EXIT)\
    X(CALL)\
    X(IF)\
    X(IFELSE)\
    X(LOOP)\
    X(NUM)\
    X(DUP)\
    X(POP)\
    X(ADD)\
    X(SUB)\
    X(MUL)\
    X(DIV)\
    X(MOD)\


//don't actually need this
enum InstrCode {
    #define X(instr) INSTR_##instr,
 
    INSTRUCTIONS
 
    #undef X
};

typedef uint8_t Instruction;
typedef int64_t Number;

typedef uint16_t seqsize_t;

#define NUMBERSIZE (sizeof(Number) / sizeof(Instruction))

Instruction *seq_next(Instruction *current);

void instr_print(Instruction i);

#endif /* INSTRUCTION_H */
