#include <stdlib.h>
#include <stdio.h>

#include "instruction.h"
#include "util.h"

#include "block.h"

typedef struct InstrList {
    Instruction instr;
    Number num;
    struct InstrList *next;
} *InstrList;

InstrList instrList_add(InstrList list, Instruction instr, Number num) {
    InstrList new = NULL;
    MALLOC(new, sizeof(*new));
    new->instr = instr;
    new->num = num;
    new->next = list;
    return new;
}

Instruction *instrList_toInstrSeq(InstrList list, blocksize_t size) {
    Instruction *new = NULL;
    MALLOC(new, size * sizeof(*new));
    for(Instruction *writer = new; list != NULL; writer = seq_next(writer)) {
        *writer = list->instr;
        if(list->instr == INSTR_NUM) {
            *(Number*)(writer+1) = list->num;
        }
        InstrList temp = list;
        list = list->next;
        free(temp);
    }
    return new;
}



Block block_generate(void) {
    blocksize_t nInstr = random() % 20;
    blocksize_t n = 0;
    
    InstrList list = instrList_add(NULL, INSTR_EXIT,0);
    
    for(blocksize_t i = 0; i < nInstr; i++) {
        Instruction i = random() % INSTR_INSTRUCTION_MAX;
        if(i == INSTR_NUM) {
            list = instrList_add(list, i, random());
            n += NUMBERSIZE + 1;
        } else {
            list = instrList_add(list, i, 0);
            n++;
        }
        
    }
    return block_new(instrList_toInstrSeq(list, n));
}

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
    printf("%li\n",*(Number*)(i+1));
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

