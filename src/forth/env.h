#ifndef ENV_H
#define ENV_H

#include "forth.h"
#include "cell.h"
#include "types.h"

void env_init(void);

identifier_t env_newID(void);


void env_move(Forth f, Cell cell, offset_t x, offset_t y);
/*
void env_eat(Forth f, Cell cell, offset_t x,y);

void env_seed(Forth f, Cell cell, offset_t x,y);
*/


#endif /* ENV_H */
