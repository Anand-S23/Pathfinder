#ifndef A_STAR_H 
#define A_STAR_H

typedef enum mode
{
    MODE_menu,
    MODE_freeplay
} mode;

typedef struct game_state
{
    memory_arena permanent_arena;
    memory_arena transient_arena;
} game_state;

#endif