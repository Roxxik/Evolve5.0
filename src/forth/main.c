#include <stdio.h>

#include "forth.h"
#include "instruction.h"


int main(void) {
    Instruction seq[] = {
        INSTR_NUM,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_NUM,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_ADD,
        INSTR_EXIT
    };
    Block main = block_new(seq);
    block_print(main);
    Code c = code_new(1,&main);
    Forth f = forth_new(c);
    printf("executing\n");
    while(forth_running(f)) {
        forth_step(f);
    }
    forth_free(f);
}
