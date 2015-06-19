#include <stdio.h>

#include "forth.h"
#include "instruction.h"

int main(void) {
    Instruction mainseq[] = {
        INSTR_NUM,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_NUM,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_CALL,
        INSTR_SQRT,
        INSTR_UNPACK2,
        INSTR_SWAP,
        INSTR_NEGATE,
        INSTR_SHL,
        INSTR_NUM,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_NEGATE,
        INSTR_RTR,
        INSTR_NUM,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_DIVMOD,
        INSTR_EXIT
    };
    Instruction twoseq[] = {
        INSTR_DUP,
        INSTR_ADD,
        INSTR_EXIT
    };
    Block main = block_new(mainseq);
    Block two = block_new(twoseq);
    Block blocks[] = { main, two };
    Code c = code_new(2,blocks);
    Forth f = forth_new(c);
    printf("executing\n");
    while(forth_running(f)) {
        forth_step(f);
    }
    forth_free(f);
}
