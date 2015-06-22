#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "instruction.h"
#include "dstack.h"

#include "env.h"

//height = 2^POWHEIGHT
//same for width
#define POWWIDTH 4
#define POWHEIGHT 4

#define FIELDWIDTH (1 << POWWIDTH)
#define FIELDHEIGHT (1 << POWHEIGHT)

#define WIDTHMASK (FIELDWIDTH - 1)
#define HEIGHTMASK (FIELDHEIGHT - 1)

typedef struct ForthList {
    Forth f;
    struct ForthList *next;
} *ForthList;

offset_t signum(Number n) {
    return (n>0) - (n<0);
}


identifier_t nextID;
Forth **Field;
ForthList Organisms;
step_t Step;

void org_add(Forth f) {
    ForthList new;
    if((new = malloc(sizeof(*new))) == NULL) { exit(1); }
    new->next = Organisms;
    new->f = f;
    Organisms = new; 
}


void env_new(Forth f) {
    if(Field[f->y][f->x] == NULL) {
        Field[f->y][f->x] = f;
        org_add(f);
    } else {
        puts("there is already someone");
        //exit(1);
        //or recurse...
        forth_free(f);
    }
}

//spawns a random organism
void env_spawn(void) {
    coord_t x,y;
    x = random() % FIELDWIDTH;
    y = random() % FIELDHEIGHT;
    static Instruction b1[] = {
        INSTR_NUM,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        INSTR_DUP,
        INSTR_DUP,
        INSTR_DEC,
        INSTR_MOVE,
        INSTR_POP,
        INSTR_LOOP
    };
    static Block blocks[] = { b1 };
    Code c = code_new(1,blocks);
    Forth f = forth_new(
        c, //use the generated code
        0, //no one is parent of this one
        0, //so it doesn't have a generation too
        Step+1, //will activate in next step
        x, //spawn random
        y,
        1 //with one energy
    );
    env_new(f);
}


void env_step(void) {
    for(ForthList fs = Organisms; fs != NULL; fs = fs->next) {
        if(forth_running(fs->f)) {
            forth_step(fs->f);
        }
    }
    printf("step: %i\n",Step);
    Step++;
}


void env_init(void) {
    nextID = 1;
    if((Field = malloc(FIELDHEIGHT * sizeof(Forth**))) == NULL) { exit(1); }
    for(coord_t i = 0; i < FIELDHEIGHT;i++) {
        if((Field[i] = calloc(FIELDWIDTH, sizeof(Forth*))) == NULL) { exit(1); }
    }
    Organisms = NULL;
    Step = 0;
    srand(time(NULL));
}

void env_fin(void) {
    for(coord_t i = 0; i < FIELDHEIGHT;i++) {
        free(Field[i]);
    }
    free(Field);
    while(Organisms != NULL) {
        ForthList temp = Organisms;
        Organisms = temp->next;
        forth_free(temp->f);
        free(temp);
    }
}

identifier_t env_newID(void) {
    return nextID++;
}

coord_t wrapX(coord_t x) {
    return x & WIDTHMASK;
}


coord_t wrapY(coord_t y) {
    return y & HEIGHTMASK;
}

void env_move(Forth f, Number x, Number y) {
    offset_t ox = signum(x);
    offset_t oy = signum(y);
    coord_t nx = wrapX(f->x + ox);
    coord_t ny = wrapY(f->y + oy);
    if(Field[ny][nx] == NULL) {
        Field[ny][nx] = f;
        Field[f->y][f->x] = NULL;
        f->x = nx;
        f->y = ny;
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
