#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "instruction.h"
#include "dstack.h"

#include "env.h"

#define FIELDWIDTH UINT8_MAX
#define FIELDHEIGHT UINT8_MAX


identifier_t nextID;
Cell **Field;


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

void env_move(Forth f, Cell cell, offset_t x, offset_t y) {
    assert(x >= -1 && x <= 1);
    assert(y >= -1 && y <= 1);
    if(Field[cell->y+y][cell->x+x] == NULL) {
        Field[cell->y+y][cell->x+x] = cell;
        Field[cell->y][cell->x] = NULL;
        cell->x = cell->x + x;
        cell->y = cell->y + y;
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
    }
}
