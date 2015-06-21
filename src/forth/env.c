#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "instruction.h"
#include "dstack.h"

#include "env.h"

#define FIELDWIDTH 10
#define FIELDHEIGHT 10


identifier_t nextID;
Forth **Field;
Forth Organisms;

void env_spawn() {
    
}

void env_step(void) {
    
}


void env_init(void) {
    nextID = 1;
    if((Field = malloc(FIELDHEIGHT * sizeof(Cell**))) == NULL) { exit(1); }
    for(coord_t i = 0; i < FIELDHEIGHT;i++) {
        if((Field[i] = calloc(FIELDWIDTH, sizeof(Cell*))) == NULL) { exit(1); }
    }
}

identifier_t env_newID(void) {
    return nextID++;
}

void env_move(Forth f, offset_t x, offset_t y) {
    assert(x >= -1 && x <= 1);
    assert(y >= -1 && y <= 1);
    if(Field[f->y+y][f->x+x] == NULL) {
        Field[f->y+y][f->x+x] = f;
        Field[f->y][f->x] = NULL;
        f->x = f->x + x;
        f->y = f->y + y;
        dstack_push(f->data, 1);
    } else {
        dstack_push(f->data, 0);
    }
}
/*
void env_eat(Forth f, Cell cell, offset_t x,y) {
    
}*/
/*
void env_seed(Forth f, Cell cell, offset_t x,y) {
    
}*/

void env_print(void) {
    for(coord_t x = 0; x < FIELDWIDTH; x++) {
        for(coord_t y = 0; y < FIELDHEIGHT; y++) {
            if(Field[y][x]) {
                putchar('#');
            } else {
                putchar(' ');
            }
        }
        puts("|");
    }
    putchar('\n');
}
