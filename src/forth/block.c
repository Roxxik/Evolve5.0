#include <stdio.h>

#include "block.h"


Block block_new(Instruction *seq) {
    return seq;
}

void block_free(Block b __attribute__((unused))) {
    //noop
}

Instruction *block_getInstrSeq(Block b) {
    return b;
}

void printNum(Instruction *i) {
    printf("%lX\n",*(Number*)(i+1));
}

void block_print(Block b) {
    Instruction *i;
    for(i = b; *i != INSTR_EXIT; i = seq_next(i)) {
        instr_print(*i);
        if(*i == INSTR_NUM) {
            printNum(i);
        }
    }
    instr_print(*i);
}
