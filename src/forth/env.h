#ifndef ENV_H
#define ENV_H

#include "forth.h"

void env_init(energy_t nrg);

void env_fin(void);

identifier_t env_newID(void);

void env_new(Forth f);

void env_spawn(void);

void env_step(void);

void env_move(Forth f, Number x, Number y);
/*
void env_eat(Forth f, Cell cell, offset_t x,y);

void env_seed(Forth f, Cell cell, offset_t x,y);
*/

void env_print(void);

#endif /* ENV_H */
