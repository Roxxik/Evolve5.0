#ifndef CODE_H
#define CODE_H

#include <stdint.h>

#include "block.h"

typedef uint8_t codesize_t;

//assume you don't know nothing about this struct
typedef struct Code {
    codesize_t size;
    Block *blocks;
} *Code;

Code code_new(codesize_t size, Block *blocks);
void code_free(Code c);

codesize_t code_getSize(Code c);

Block code_getBlock(Code c, codesize_t index);


#endif /* CODE_H */
