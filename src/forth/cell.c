#include <stdlib.h>

#include "env.h"

#include "cell.h"

Cell cell_new(identifier_t parentID, generation_t parentGen, step_t step, coord_t x, coord_t y, energy_t nrg) {
    Cell c;
    if((c = malloc(sizeof(*c))) == NULL) { exit(1); }
    c->id = env_newID();
    c->parentID = parentID;
    c->generation = parentGen + 1;
    c->step = step;
    c->x = x;
    c->y = y;
    c->energy = nrg;
    c->used = 0;
    return c;
}

void cell_free(Cell c) {
    free(c);
}
