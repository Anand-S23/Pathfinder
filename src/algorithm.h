#ifndef ALGORITHM_H
#define ALGORITHM_H

typedef struct algorithm_t
{
    linked_list list;
    cell *current;
    b32 initialized;
} algorithm_t;

#endif
