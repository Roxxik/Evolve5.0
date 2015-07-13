#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

#include "instruction.h"

typedef uint16_t blocksize_t;

typedef Instruction *Block;
//this might be replaced later to include a refcount and a mutex
// -> do not assume by dereferencing a block you'll get a valid instruction sequence

Block block_new(Instruction *seq);
void block_free(Block b);

/**
 * returns a pointer to the first instruction
 * only read from this pointer or follow the Sequence with nextInstruction
 */
Instruction *block_getInstrSeq(Block b);

/**
 * just print the block, mostly for debugging purposes
 */
void block_print(Block b);

Block block_generate(void);

Block block_copy(Block b);

Block block_mutate(Block b);

#endif /* BLOCK_H */
