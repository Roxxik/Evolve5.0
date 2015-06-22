#include <stdio.h>

#include "env.h"

int main(void) {
    env_init();
    Code c = code_generate();
    code_print(c);
    code_free(c);
    env_fin();
    /*env_init();
    env_spawn();
    env_print();
    int c = 0;
    while (c != 'x') {
        env_spawn();
        env_step();
        env_print();
        c = getchar();
    }
    env_fin();*/
}
