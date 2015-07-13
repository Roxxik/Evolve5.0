#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

blocksize_t instrList_length(InstrList list) {
    blocksize_t res = 0;
    for(;list != NULL; list = list->next) {
        ++res;
    }
    return res;
}

blocksize_t instrList_size(InstrList list) {
    blocksize_t res = 0;
    for(;list != NULL; list = list->next) {
        if(list->instr == INSTR_NUM) {
            res += NUMBERSIZE;
        }
        ++res;
    }
    return res;
}

InstrList instrList_get(InstrList list, blocksize_t index) {
    for(;list != NULL &&  index > 0; list = list->next, index--);
    return list;
}

void instrList_reverse(InstrList *list) {
    InstrList prev = NULL;
    InstrList current = *list;
    InstrList next;
    while(current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *list = prev;
}

//this will delete the list
Instruction *instrList_toInstrSeq(InstrList list) {
    Instruction *new = NULL;
    MALLOC(new, instrList_size(list) * sizeof(*new));
    for(Instruction *writer = new; list != NULL;) {
        *writer = list->instr;
        if(list->instr == INSTR_NUM) {
            *(Number*)(writer+1) = list->num;
        }
        InstrList temp = list;
        list = list->next;
        free(temp);
        if(list != NULL) {
            writer = seq_next(writer);
        }
    }
    return new;
}

//this keeps the seq intact
InstrList instrList_fromInstrSeq(Instruction *seq) {
    InstrList list = NULL;
    for(Instruction *i = seq; *i != INSTR_EXIT; i = seq_next(i)) {
        if(*i == INSTR_NUM) {
            list = instrList_add(list, *i, *(Number*)(i+1));
        } else {
            list = instrList_add(list, *i, 0);
        }
    }
    list = instrList_add(list, INSTR_EXIT, 0);
    instrList_reverse(&list);
    return list;
}

Block block_generate(void) {
    blocksize_t nInstr = random() % 100;
    
    InstrList list = instrList_add(NULL, INSTR_EXIT, 0);
    
    for(blocksize_t i = 0; i < nInstr; i++) {
        Instruction i = random() % INSTR_INSTRUCTION_MAX;
        assert(i >= 0 && i < INSTR_INSTRUCTION_MAX);
        if(i == INSTR_NUM) {
            list = instrList_add(list, i, random());
        } else {
            list = instrList_add(list, i, 0);
        }
    }
    return block_new(instrList_toInstrSeq(list));
}


Block block_copy(Block b) {
    return block_new(instrList_toInstrSeq(instrList_fromInstrSeq(block_getInstrSeq(b))));
}

Block block_mutate(Block b) {
    InstrList list = instrList_fromInstrSeq(block_getInstrSeq(b));
    blocksize_t length = instrList_length(list);
    
    blocksize_t nInstr = random() % length;
    
    InstrList elem = instrList_get(list,nInstr);
    
    assert(elem != NULL);
    
    elem->instr = random() % INSTR_INSTRUCTION_MAX;
    if(elem->instr == INSTR_NUM) {
        elem->num = random();
    }
    
    return block_new(instrList_toInstrSeq(list));   
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

