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
    \
    X(EXEC)\
    \
    X(EXIT)\
    X(CALL)\
    X(IF)\
    X(IFELSE)\
    X(LOOP)\
    X(CONDEXIT)\
    X(CB)\
    \
    X(NUM)\
    \
    X(DUP)\
    X(POP)\
    X(SWAP)\
    X(OVER)\
    X(ROT)\
    X(TOR)\
    X(CONDDUP)\
    \
    X(INC)\
    X(DEC)\
    X(INCTWO)\
    X(DECTWO)\
    X(HALF)\
    X(DOUBLE)\
    X(ABS)\
    X(SQRT)\
    X(CBRT)\
    X(ZERO)\
    X(NEGATE)\
    X(NOT)\
    X(INVERT)\
    X(SGN)\
    X(SQUARE)\
    X(CUBE)\
    \
    X(ADD)\
    X(SUB)\
    X(MUL)\
    X(DIV)\
    X(MOD)\
    X(DIVMOD)\
    X(EQ)\
    X(NE)\
    X(LT)\
    X(GT)\
    X(LE)\
    X(GE)\
    X(OR)\
    X(AND)\
    X(LOR)\
    X(LAND)\
    X(XOR)\
    X(MIN)\
    X(MAX)\
    X(SHR)\
    X(SHL)\
    X(RTR)\
    X(RTL)\
    \
    X(PACK2)\
    X(UNPACK2)\
    X(PACK4)\
    X(UNPACK4)\
    \
    X(RN)\
    X(R0)\
    X(R1)\
    X(R2)\
    X(R3)\
    X(R4)\
    X(R5)\
    X(R6)\
    X(R7)\
    X(R8)\
    X(R9)\
    X(SETRN)\
    X(SETR0)\
    X(SETR1)\
    X(SETR2)\
    X(SETR3)\
    X(SETR4)\
    X(SETR5)\
    X(SETR6)\
    X(SETR7)\
    X(SETR8)\
    X(SETR9)\
    \
    X(MOVE)\
    X(EAT)\
    X(SEED)\
    \
    X(INSTRUCTION_MAX)


//don't actually need this
enum InstrCode {
    #define X(instr) INSTR_##instr,
 
    INSTRUCTIONS
 
    #undef X
};

#define REGISTER_MAX 10

#define REGISTERS \
    X(0)\
    X(1)\
    X(2)\
    X(3)\
    X(4)\
    X(5)\
    X(6)\
    X(7)\
    X(8)\
    X(9)\

typedef uint8_t Instruction;
typedef int64_t Number;
typedef uint64_t UNumber;

typedef uint16_t seqsize_t;

#define NUMBERSIZE (sizeof(Number) / sizeof(Instruction))

Instruction *seq_next(Instruction *current);

void instr_print(Instruction i);

#endif /* INSTRUCTION_H */
