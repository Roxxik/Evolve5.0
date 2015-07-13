#ifndef ENV_H
#define ENV_H

#include "forth.h"

void env_setStepSize(step_t newsize);

void env_init(energy_t nrg);

void env_fin(void);

identifier_t env_newID(void);

void env_new(Forth f);

void env_spawn(energy_t nrg);

void env_step(void);

void env_move(Forth f, Number x, Number y);

void env_eat(Forth f, Number x, Number y);

void env_seed(Forth f, Number x, Number y, Number nrg);


void env_print(void);

#endif /* ENV_H */
