#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>

#define MALLOC(var, size) \
do {\
    if((var = malloc(size)) == NULL) {\
        printf(\
            "error: Out Of Memory, %s: %s: %d",\
            __FILE__,\
            __FUNCTION__,\
            __LINE__\
        );\
        exit(1);\
    }\
} while(0)


#endif /* UTIL_H */
