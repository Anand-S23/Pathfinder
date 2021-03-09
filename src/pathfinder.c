#define GS_IMPL
#include <gs.h>

#define GS_IMMEDIATE_DRAW_IMPL
#include <util/gs_idraw.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#include "pathfinder.h"
#include "data_structures.c"
#include "algorithm.c"
#include "ui.c"

global app_state state = {0};

internal void ResetMap(app_state *state)
{
    for (int j = 0; j < MAP_H; ++j)
    {
        for (int i = 0; i < MAP_W; ++i)
        {
            state->map[j][i].type = CELL_TYPE_open;

            b32 north = 0;
            b32 south = 0;
            b32 east = 0;
            b32 west = 0;

            if (j == 0)
            {
                north = 1;
            }
            else if (j == MAP_H - 1)
            {
                south = 1;
            }

            
            if (i == 0)
            {
                west = 1;
            }
            else if (i == MAP_W - 1)
            {
                east = 1;
            }


            state->map[j][i].walls[NORTH] = north;
            state->map[j][i].walls[SOUTH] = south;
            state->map[j][i].walls[EAST] = east;
            state->map[j][i].walls[WEST] = west;
        }
    }

    state->start.j = 0;
    state->start.i = 0;
    state->end.j = 17;
    state->end.i = 17;
}

internal void ClearMap(app_state *state)
{
    for (int j = 0; j < MAP_H; ++j)
    {
        for (int i = 0; i < MAP_W; ++i)
        {
            if (state->map[j][i].type == CELL_TYPE_visited ||
                state->map[j][i].type == CELL_TYPE_path)
            {
                state->map[j][i].type = CELL_TYPE_open;
            }
        }
    }
}

internal void HandleEvent(app_state *state, int mouse_x, int mouse_y)
{
    if (mouse_x > 0 && mouse_x < 720 &&
        mouse_y > 0 && mouse_y < 720 &&
        !state->pathfinding)
    {
        int i = mouse_x / CELL_W; 
        int j = mouse_y / CELL_H;

        direction ns, ew, dir_final;
        int ns_val, ew_val;

        if (mouse_x - i * CELL_W > (i + 1) * CELL_W - mouse_x)
        {
            ew = EAST;
            ew_val = mouse_x - i * CELL_W;
        }
        else
        {
            ew = WEST;
            ew_val = (i + 1) * CELL_W - mouse_x;
        }
            
        if (mouse_y - j * CELL_H > (j + 1) * CELL_H - mouse_y)
        {
            ns = SOUTH;
            ns_val = mouse_y - j * CELL_H;
        }
        else
        {
            ns = NORTH;
            ns_val = (j + 1) * CELL_H - mouse_y;
        }

        dir_final = (ew_val > ns_val) ? ew : ns;

        if (dir_final == NORTH && j != 0)
        {
            state->hover.i = i;
            state->hover.j = j;
            state->hover.wall = NORTH;
        }
        else if (dir_final == SOUTH && j != MAP_H - 1)
        {
            state->hover.i = i;
            state->hover.j = j;
            state->hover.wall = SOUTH;
        }
        else if (dir_final == WEST && i != 0)
        {
            state->hover.i = i;
            state->hover.j = j;
            state->hover.wall = WEST;
        }
        else if (dir_final == EAST && i != MAP_W - 1)
        {
            state->hover.i = i;
            state->hover.j = j;
            state->hover.wall = EAST;
        }
            

        if (gs_platform_mouse_down(GS_MOUSE_LBUTTON))
        {
            if (gs_platform_key_down(GS_KEYCODE_LSHIFT))
            { 
                // Place start if cell is not end
                if (state->end.i != i && state->end.j != j)
                {
                    state->start.i = i;
                    state->start.j = j;
                }
            }
            else
            {
                // Place wall
                if (state->hover.i != -1 && state->hover.j != -1)
                {
                    if (state->hover.wall == NORTH)
                    {
                        state->map[state->hover.j - 1][state->hover.i].walls[SOUTH] = 1; 
                        state->map[state->hover.j][state->hover.i].walls[NORTH] = 1; 
                    }

                    else if (state->hover.wall == SOUTH)
                    {
                        state->map[state->hover.j + 1][state->hover.i].walls[NORTH] = 1; 
                        state->map[state->hover.j][state->hover.i].walls[SOUTH] = 1; 
                    }

                    else if (state->hover.wall == EAST)
                    {
                        state->map[state->hover.j][state->hover.i + 1].walls[WEST] = 1; 
                        state->map[state->hover.j][state->hover.i].walls[EAST] = 1; 
                    }

                    else if (state->hover.wall == WEST)
                    {
                        state->map[state->hover.j][state->hover.i - 1].walls[EAST] = 1; 
                        state->map[state->hover.j][state->hover.i].walls[WEST] = 1; 
                    }
                }
            }
        }

        else if (gs_platform_mouse_down(GS_MOUSE_RBUTTON))
        {
            if (gs_platform_key_down(GS_KEYCODE_LSHIFT))
            { 
                // Place end if cell is not start
                if (state->start.i != i && state->start.j != j)
                {
                    state->end.i = i;
                    state->end.j = j;
                }
            }
            else
            {
                // Remove wall 
                if (state->hover.i != -1 && state->hover.j != -1)
                {
                    if (state->hover.wall == NORTH)
                    {
                        state->map[state->hover.j - 1][state->hover.i].walls[SOUTH] = 0; 
                        state->map[state->hover.j][state->hover.i].walls[NORTH] = 0; 
                    }

                    else if (state->hover.wall == SOUTH)
                    {
                        state->map[state->hover.j + 1][state->hover.i].walls[NORTH] = 0; 
                        state->map[state->hover.j][state->hover.i].walls[SOUTH] = 0; 
                    }

                    else if (state->hover.wall == EAST)
                    {
                        state->map[state->hover.j][state->hover.i + 1].walls[WEST] = 0; 
                        state->map[state->hover.j][state->hover.i].walls[EAST] = 0; 
                    }

                    else if (state->hover.wall == WEST)
                    {
                        state->map[state->hover.j][state->hover.i - 1].walls[EAST] = 0; 
                        state->map[state->hover.j][state->hover.i].walls[WEST] = 0; 
                    }
                }
            }
        }
    }
    else
    {
        state->hover.i = -1;
        state->hover.j = -1;
    }
}

internal void RenderMap(app_state *state)
{
    // Renderer the start cell
    gsi_rectv(&state->renderer,
              gs_v2(state->start.i * CELL_W + CELL_W,
                    state->start.j * CELL_H + CELL_H),
              gs_v2(state->start.i * CELL_W,
                    state->start.j * CELL_H),
              GS_COLOR_GREEN, GS_GRAPHICS_PRIMITIVE_TRIANGLES); 

    // Renderer the end cell
    gsi_rectv(&state->renderer,
              gs_v2(state->end.i * CELL_W + CELL_W,
                    state->end.j * CELL_H + CELL_H),
              gs_v2(state->end.i * CELL_W,
                    state->end.j * CELL_H),
              GS_COLOR_BLUE, GS_GRAPHICS_PRIMITIVE_TRIANGLES); 

    // Render faded grid
    for (int j = 0; j < MAP_H; ++j)
    {
            gsi_line(&state->renderer,
                     j * CELL_W, 0, j * CELL_W, 720,
                     90, 90, 90, 255);

            gsi_line(&state->renderer,
                     0, j * CELL_H, 720, j * CELL_H,
                     90, 90, 90, 255);
    }

    // Map
    for (int j = 0; j < MAP_H; ++j)
    {

        for (int i = 0; i < MAP_W; ++i)
        {
            // Render visited && path
            if (state->map[j][i].type == CELL_TYPE_visited)
            {
                gsi_rectv(&state->renderer,
                          gs_v2(i * CELL_W + CELL_W, j * CELL_H + CELL_H),
                          gs_v2(i * CELL_W, j * CELL_H),
                          gs_color(255, 255, 0, 255), GS_GRAPHICS_PRIMITIVE_TRIANGLES);
            }
            else if (state->map[j][i].type == CELL_TYPE_path)
            {
                gsi_rectv(&state->renderer,
                          gs_v2(i * CELL_W + CELL_W, j * CELL_H + CELL_H),
                          gs_v2(i * CELL_W, j * CELL_H),
                          gs_color(0, 255, 255, 255), GS_GRAPHICS_PRIMITIVE_TRIANGLES);
            }

            // Render walls
            if (state->map[j][i].walls[SOUTH])
            {
                gsi_line(&state->renderer,
                         i * CELL_W, (j + 1) * CELL_H,
                         (i + 1) * CELL_W, (j + 1) * CELL_H,
                         255, 255, 255, 255);
            }

            if (state->map[j][i].walls[EAST])
            {
                gsi_line(&state->renderer,
                         (i + 1) * CELL_W, j * CELL_H,
                         (i + 1) * CELL_W, (j + 1) * CELL_H,
                         255, 255, 255, 255);
            }
        }
    }

    if (state->hover.i != -1 && state->hover.j != -1)
    {
        if (state->hover.wall == NORTH)
        {
            gsi_line(&state->renderer,
                     state->hover.i * CELL_W, state->hover.j * CELL_H,
                     (state->hover.i + 1) * CELL_W, state->hover.j * CELL_H,
                     255, 255, 0, 255);
        }

        else if (state->hover.wall == SOUTH)
        {
            gsi_line(&state->renderer,
                     state->hover.i * CELL_W, (state->hover.j + 1) * CELL_H,
                     (state->hover.i + 1) * CELL_W, (state->hover.j + 1) * CELL_H,
                     255, 255, 0, 255);
        }
        
        else if (state->hover.wall == EAST)
        {
            gsi_line(&state->renderer,
                     (state->hover.i + 1) * CELL_W, state->hover.j * CELL_H,
                     (state->hover.i + 1) * CELL_W, (state->hover.j + 1) * CELL_H,
                     255, 255, 0, 255);
        }

        else if (state->hover.wall == WEST)
        {
            gsi_line(&state->renderer,
                     state->hover.i * CELL_W, state->hover.j * CELL_H,
                     state->hover.i * CELL_W, (state->hover.j + 1) * CELL_H,
                     255, 255, 0, 255);
        }
    }
}

internal void InitApp()
{
    state.command_buffer = gs_command_buffer_new();
    state.renderer = gs_immediate_draw_new();
    // gs_asset_font_load_from_file("./assets/UM_Bold.ttf", &state.header_font, 48);
    // gs_asset_font_load_from_file("./assets/UM.ttf", &state.font, 24);
    ResetMap(&state);
    srand(time(0));
}

internal void UpdateApp()
{
    int mouse_x, mouse_y;
    gs_platform_mouse_position(&mouse_x, &mouse_y);

    input ui_input = {0};
    {
        ui_input.mouse_x = mouse_x;
        ui_input.mouse_y = mouse_y;
        ui_input.left_mouse_down = gs_platform_mouse_down(GS_MOUSE_LBUTTON);
        ui_input.right_mouse_down = gs_platform_mouse_down(GS_MOUSE_RBUTTON);
    }

    // Update map with user input
    HandleEvent(&state, mouse_x, mouse_y);
            
    // Clear Screen
    gsi_render_pass_submit(&state.renderer, &state.command_buffer,
                           gs_color(75, 75, 75, 255));

    gsi_camera2D(&state.renderer);

    // Renderer the map
    RenderMap(&state);

    // UI
    UIBeginFrame(&state.ui, &state.renderer, &state.command_buffer,
                 &ui_input, &state.font);
    {
        local_persist ui_option algorithm_option = {0};

        if (!algorithm_option.initalized)
        {
            algorithm_option =
                UIOption(gs_color(255, 204, 18, 255), gs_v2(40, 40));
            algorithm_option.initalized = 1;
        }

        ui_id dfs = UIOptionButton(&state.ui, &algorithm_option, UIIDGen(),
                                   "DFS", gs_v2(740.f, 75.f));

        ui_id bfs = UIOptionButton(&state.ui, &algorithm_option, UIIDGen(),
                                   "BFS", gs_v2(740.f, 150.f));

        ui_id dj = UIOptionButton(&state.ui, &algorithm_option, UIIDGen(),
                                  "Dj", gs_v2(740.f, 225.f));

        ui_id a_star = UIOptionButton(&state.ui, &algorithm_option, UIIDGen(),
                                      "A Star", gs_v2(740.f, 300.f));


        if (state.pathfinding)
        {
            // Pathfinding
            if (state.finding_algo == ALGO_dfs)
            {
                DFSPathfinding(&state);
                Sleep(50);
            }

            gsi_rectv(&state.renderer, gs_v2(980.f, 440.f), gs_v2(740.f, 375.f),
                      gs_color(100, 100, 100, 255), GS_GRAPHICS_PRIMITIVE_TRIANGLES);

            if (UICustomButton(&state.ui, UIIDGen(), "Stop Generating",
                               gs_v2(980.f, 590.f), gs_v2(740.f, 525.f),
                               gs_color(96, 168, 107, 255)))
            {
                state.pathfinding = 0;
                ClearMap(&state);
            }
        }
        else
        {
            if (UIButton(&state.ui, UIIDGen(), "Submit",
                         gs_v2(980.f, 440.f), gs_v2(740.f, 375.f)))
            {
                if (UIIDEqual(algorithm_option.selected, dfs))
                {
                    state.finding_algo = ALGO_dfs;
                    state.pathfinding = 1;
                }
            }

            if (UICustomButton(&state.ui, UIIDGen(), "Clear", gs_v2(980.f, 540.f),
                            gs_v2(740.f, 475.f), gs_color(96, 168, 107, 255)))
            {
                ClearMap(&state);
            }

            if (UICustomButton(&state.ui, UIIDGen(), "Reset", gs_v2(980.f, 640.f),
                               gs_v2(740.f, 575.f), gs_color(254, 116, 85, 255)))
            {
                ResetMap(&state);
            }
        }
    }
    UIEndFrame(&state.ui);

    /*
    gsi_text(&state.renderer, 740.f, 40.f, "Pathfinder",
            &state.header_font, false, 255, 255, 255, 255);
    */

    gs_graphics_submit_command_buffer(&state.command_buffer);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    gs_app_desc_t app_desc = {0};
    {
        app_desc.window_title = "Pathfinder";
        app_desc.window_width = 1000;
        app_desc.window_height = 720;
        app_desc.init = InitApp;
        app_desc.update = UpdateApp;
    }

    return app_desc;
}   
