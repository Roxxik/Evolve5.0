#ifndef CELL_H
#define CELL_H

#include <stdint.h>

#include "types.h"

typedef struct Cell {
    identifier_t id;
    identifier_t parentID;
    generation_t generation;
    step_t step;//maybe include an epoch too for more steps
    coord_t x,y;
    energy_t energy;
    energy_t used;
} *Cell;

Cell cell_new(identifier_t parentID, generation_t parentGen, step_t step, coord_t x, coord_t y, energy_t nrg);
void cell_free(Cell c);


#endif /* CELL_H */
