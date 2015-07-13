#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "instruction.h"
#include "dstack.h"
#include "util.h"

#include "env.h"

//height = 2^POWHEIGHT
//same for width
#define POWWIDTH 5
#define POWHEIGHT 6

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
step_t StepSize;
energy_t Energy;

void env_setStepSize(step_t newsize) {
    StepSize = newsize;
}

void org_add(Forth f) {
    ForthList new;
    MALLOC(new, sizeof(*new));
    new->next = Organisms;
    new->f = f;
    Organisms = new; 
}


void env_new(Forth f) {
    if(Field[f->y][f->x] == NULL) {
        Field[f->y][f->x] = f;
        org_add(f);
        Energy -= f->energy;
    } else {
        //puts("there is already someone");
        //exit(1);
        //or recurse...
        forth_free(f);
    }
}

//spawns a random organism
void env_spawn(energy_t nrg) {
    coord_t x,y;
    x = random() % FIELDWIDTH;
    y = random() % FIELDHEIGHT;
    Code c = code_generate();
    Forth f = forth_new(
        c, //use the generated code
        0, //no one is parent of this one
        0, //so it doesn't have a generation too
        Step+1, //will activate in next step
        x, //spawn random
        y,
        nrg //with one energy
    );
    env_new(f);
}

void forthList_free(ForthList fs) {
    Forth f = fs->f;
    Energy += f->energy;
    assert(f->x >= 0 && f->x < FIELDWIDTH);
    assert(f->y >= 0 && f->y < FIELDHEIGHT);
    Field[f->y][f->x] = NULL;
    forth_free(f);
    free(fs);
}

ForthList wreckZombies(ForthList list) {
    while(list != NULL && !forth_running(list->f) && list->f->energy == 0) {
        ForthList temp = list->next;
        forthList_free(list);
        list = temp;
    }
    ForthList fs = list;
    while(fs != NULL) {
        if(fs->next != NULL && !forth_running(fs->next->f)  && fs->next->f->energy == 0) {
            ForthList temp = fs->next;
            fs->next = temp->next;
            forthList_free(temp);
        } else {
            fs = fs->next;
        }
    }
    return list;
}

uint16_t killcounter;

void env_step(void) {
    Organisms = wreckZombies(Organisms);
    for(ForthList fs = Organisms; fs != NULL; fs = fs->next) {
        if(forth_running(fs->f)) {
            forth_step(fs->f);
        }
    }
    /*
    if(Energy > 0) {
        env_spawn(1);
    }*/
    /*
    energy_t nrg = 1 + random() % 9;
    if(Energy >= nrg) {
        env_spawn(nrg);
    } else {
        killcounter++;
    }
    if(killcounter > 1024) {
        coord_t x,y;
        x = random() % FIELDWIDTH;
        y = random() % FIELDHEIGHT;
        if(Field[y][x] != NULL) {
            forth_exit(Field[y][x]);
        }
        killcounter = 0;
    }*/
    /*
    printf("step: %i\n",Step);
    printf("energy: %i\n",Energy);
    printf("count: %i\n",killcounter);
    */
    
    if((Step & ~(~0 << StepSize)) == 0) {
        printf("step: %i\n",Step);
        printf("energy: %i\n",Energy);
        printf("count: %i\n",killcounter);
        env_print();
        
        int c = getchar();
        step_t stepSize;      
        if(c != '\n') {
            ungetc(c,stdin);
            scanf("%u",&stepSize);
            env_setStepSize(stepSize);
        }
    }
    Step++;
}


void env_init(energy_t nrg) {
    assert(nrg <= FIELDHEIGHT * FIELDWIDTH);
    nextID = 1;
    MALLOC(Field,FIELDHEIGHT * sizeof(Forth**));
    for(coord_t i = 0; i < FIELDHEIGHT;i++) {
        if((Field[i] = calloc(FIELDWIDTH, sizeof(Forth*))) == NULL) { exit(1); }
    }
    Organisms = NULL;
    Step = 0;
    StepSize = 0;
    Energy = nrg;
    killcounter = 0;
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

void env_eat(Forth f, Number x, Number y) {
    offset_t ox = signum(x);
    offset_t oy = signum(y);
    coord_t nx = wrapX(f->x + ox);
    coord_t ny = wrapY(f->y + oy);
    if(Field[ny][nx] != NULL) {
        Forth v = Field[ny][nx];
        forth_exit(v);
        energy_t nrg = v->energy;
        v->energy = 0;
        f->energy += nrg;
        dstack_push(f->data, nrg);
    } else {
        dstack_push(f->data, 0);
    }
}


void env_seed(Forth f, Number x, Number y, Number nrg) {
    offset_t ox = signum(x);
    offset_t oy = signum(y);
    coord_t nx = wrapX(f->x + ox);
    coord_t ny = wrapY(f->y + oy);
    if(nrg > 0 && Field[ny][nx] == NULL) {
        Forth n = forth_seed(f,nx,ny,nrg);
        if(n != NULL) {
            Field[ny][nx] = n;
            org_add(n);
            dstack_push(f->data, 1);
        } else {
            dstack_push(f->data, 0);
        }
    } else {
        dstack_push(f->data, 0);
    }
}

void env_print(void) {
    for(coord_t x = 0; x < FIELDWIDTH; x++) {
        for(coord_t y = 0; y < FIELDHEIGHT; y++) {
            if(Field[y][x]) {
                if(forth_running(Field[y][x])) {
                    printf("%3d",Field[y][x]->energy);
                } else {
                    printf("  X");
                }
            } else {
                printf("   ");
            }
        }
        puts("|");
    }
    putchar('\n');
}
