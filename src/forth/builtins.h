#ifndef BUILTINS_H
#define BUILTINS_H

#include "instruction.h"
#include "forth.h"

#define X(name) void instr##name(Forth f);

INSTRUCTIONS

#undef X

#endif /* BUILTINS_H */
