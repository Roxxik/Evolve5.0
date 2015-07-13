#include <stdio.h>

#include "env.h"
#include "instruction.h"
#include "util.h"

int main(void) {
    env_init(400);
    Instruction i[] = { 
        INSTR_NUM,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_DUP,
        INSTR_DUP2,
        INSTR_DUP2,
        INSTR_DUP2,
        INSTR_EAT,
        INSTR_POP,
        INSTR_SEED,
        INSTR_POP,
        INSTR_NEGATE,
        INSTR_DUP2,
        INSTR_MOVE,
        INSTR_POP,
        INSTR_EAT,
        INSTR_POP,
        INSTR_LOOP,
        INSTR_EXIT
    };
    Block b1 = block_new(i);
    Block b2 = block_copy(b1);
    Block *b;
    MALLOC(b,sizeof(*b));
    *b = b2;
    Code code = code_new(1,b);
    Forth f = forth_new(code,0,0,0,5,5,400);
    env_new(f);
    env_setStepSize(2);
    while(1) {
        env_step();
    }
    env_fin();
}
