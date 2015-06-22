#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "block.h"

#include "code.h"

Code code_new(codesize_t size, Block *blocks) {
    Code c;
    if ((c = malloc(sizeof(*c))) == NULL) { exit(1); };
    c->size = size;
    c->blocks = blocks;
    return c;
}
void code_free(Code c) {
    for(codesize_t i = 0; i < code_getSize(c); i++) {
        block_free(code_getBlock(c,i));
        //better: dec ref
    }
    free(c);
}

codesize_t code_getSize(Code c) {
    return c->size;
}

Block code_getBlock(Code c, codesize_t index) {
    assert(index >= 0 && index < code_getSize(c));
    return c->blocks[index];
}
