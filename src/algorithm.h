#ifndef ALGORITHM_H
#define ALGORITHM_H

/* Depth First Search */
typedef struct dfs
{
    linked_list path_stack;
    cell *current;
    b32 initialized;
} dfs;

typedef struct bfs
{
    linked_list path_queue;
    cell *current;
    b32 initialized;
} bfs;

#endif
