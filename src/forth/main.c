#include <stdio.h>

#include "env.h"

int main(void) {
    env_init(2047);
    int c = 0;
    while(c != 'x') {
        env_step();
        //env_print();
        //c = getchar();
    }
    env_fin();
}
