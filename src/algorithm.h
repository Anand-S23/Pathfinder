#ifndef ALGORITHM_H
#define ALGORITHM_H

/* Depth First Search */
typedef struct dfs
{
    b32 initialized;
    linked_list path_stack;
    cell *current;
} dfs;

#endif
