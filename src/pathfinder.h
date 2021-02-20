#ifndef PATHFINDER_H
#define PATHFINDER_H 

#include <stdint.h> 
#include "ui.h"

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef i32 b32;

#define internal static
#define global static
#define local_persist static

#define ArraySize(arr) (sizeof(arr) / sizeof((arr)[0]))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define AbsoluteValue(a) ((a) > 0 ? (a) : -(a))

#define MAP_W (720 / 40)
#define MAP_H (720 / 40)
#define MAX   (GRID_W * GRID_H)

#define CELL_W (720 / MAP_W)
#define CELL_H (720 / MAP_H)

typedef enum cell_type
{
    CELL_TYPE_open,
    CELL_TYPE_visited,
    CELL_TYPE_obstacle
} cell_type;

typedef struct input
{
    int mouse_x;
    int mouse_y;
    b32 left_mouse_down;
    b32 right_mouse_down;
} input;

typedef struct cell
{
    int j;
    int i;
    cell_type type;

    struct cell *parent;
    int neighbor_count;
    struct cell neighbors[8];
} cell;

typedef struct app_state
{
    gs_command_buffer_t command_buffer;
    gs_immediate_draw_t renderer;

    ui ui;

    cell_type map[MAP_H][MAP_W];
    cell start;
    cell end;
} app_state; 

#endif
