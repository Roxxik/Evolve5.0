#include <stdlib.h>
#include <stdio.h>

#include "block.h"


Block block_new(Instruction *seq) {
    return seq;
}

void block_free(Block b) {
    free(b);
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

Block block_generate(void) {
    blocksize_t nInstr = random() % 20;
    Instruction *instr = NULL;
    if((instr = malloc((nInstr + 1) * sizeof(*instr))) == NULL) { exit(1); }
    for(blocksize_t i = 0; i < nInstr; i++) {
        instr[i] = random() % INSTR_INSTRUCTION_MAX;
    }
    instr[nInstr] = INSTR_EXIT;
    return block_new(instr);
}
