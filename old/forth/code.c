#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "block.h"
#include "util.h"

#include "code.h"

Code code_new(codesize_t size, Block *blocks) {
    Code c;
    MALLOC(c,sizeof(*c));
    c->size = size;
    c->blocks = blocks;
    return c;
}
void code_free(Code c) {
    for(codesize_t i = 0; i < code_getSize(c); i++) {
        block_free(code_getBlock(c,i));
        //if using refC: dec ref
    }
    free(c->blocks);
    free(c);
}

codesize_t code_getSize(Code c) {
    return c->size;
}

Block code_getBlock(Code c, codesize_t index) {
    assert(index >= 0 && index < code_getSize(c));
    return c->blocks[index];
}

void code_print(Code c) {
    for(codesize_t i = 0; i < code_getSize(c);i++) {
        printf("%d:\n",i);
        block_print(code_getBlock(c,i));
    }
}

Code code_generate(void) {
    codesize_t nBlocks = 1 + (random() % 20);
    Block *blocks;
    MALLOC(blocks,nBlocks * sizeof(*blocks));
    for(codesize_t i = 0; i < nBlocks; i++) {
        blocks[i] = block_generate();
    }
    return code_new(nBlocks,blocks);
}


Code code_mutate(Code c) {
    codesize_t size = code_getSize(c);
    codesize_t nBlock = random() % size;
    bool mutate = (random() % 10) == 0;
    Block *blocks;
    MALLOC(blocks,size * sizeof(*blocks));
    for(codesize_t i = 0; i < size; i++) {
        if(i == nBlock && mutate) {
            blocks[i] = block_mutate(code_getBlock(c,i));
        } else {
            blocks[i] = block_copy(code_getBlock(c,i));
        }
    }
    return code_new(size,blocks);
}
