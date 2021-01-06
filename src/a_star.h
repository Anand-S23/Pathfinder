#ifndef A_STAR_H 
#define A_STAR_H

typedef enum mode
{
    MODE_menu,
    MODE_editor, 
    MODE_finding
} mode;

typedef enum cell_type
{
    TYPE_walkable,
    TYPE_visited,
    TYPE_obstacle
} cell_type;

typedef struct cell
{
    int i; 
    int j;

    cell_type type;
    f32 f_local_cost; 
    f32 f_global_cost;
    struct cell* parent;
    struct cell* neighbors[8];
    int neighbors_count;
} cell;

#include "list.h"

typedef struct app_state
{
    memory_arena permanent_arena;
    memory_arena transient_arena;

    mode current_mode;
    cell grid[GRID_W][GRID_H];
    linked_list open;

    cell *start;
    cell *end;
} app_state;

#endif