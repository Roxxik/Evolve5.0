#include <stdio.h>

#include "instruction.h"

/**
 * returns size of sequence in multiple of sizeof(Instruction)
 */
seqsize_t seq_getSize(Instruction *seq) {
    seqsize_t cnt = 1;//There is always at least the exit
    while(*seq != INSTR_EXIT) {
        if(*seq == INSTR_NUM) {
            cnt += NUMBERSIZE;
        }
        cnt++;
        seq = seq_next(seq);
    }
    return cnt;
}

Instruction *seq_next(Instruction *current) {
    if(*current == INSTR_NUM) {
        current += NUMBERSIZE;
    }
    return current + 1;
}


void instr_print(Instruction i) {
    switch(i) {
        #define X(instr) case INSTR_##instr: printf("%s(%d)\n", #instr, INSTR_##instr);break;
        
        INSTRUCTIONS
        
        #undef X
        default: printf("%s(%d)\n", "NOOP", i);
    }
}
