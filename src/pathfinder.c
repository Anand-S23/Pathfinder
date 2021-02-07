#define GS_IMPL
#include <gs.h>

#define GS_IMMEDIATE_DRAW_IMPL
#include <util/gs_idraw.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pathfinder.h"
#include "ui.c"

global app_state state = {0};

internal void InitApp()
{
    state.command_buffer = gs_command_buffer_new();
    state.renderer = gs_immediate_draw_new();
}

internal b32 CellIsValid(int mouse_x, int mouse_y)
{
    return (mouse_x >= 0 && mouse_x <= MAP_W &&
            mouse_y >= 0 && mouse_y <= MAP_H);
}

internal void UpdateApp()
{
    int mouse_x, mouse_y;
    gs_platform_mouse_position(&mouse_x, &mouse_y);
    // printf("x: %d, y: %d\n", mouse_x, mouse_y);

    input ui_input = {0};
    {
        ui_input.mouse_x = mouse_x;
        ui_input.mouse_y = mouse_y;
        ui_input.left_mouse_down = gs_platform_mouse_down(GS_MOUSE_LBUTTON);
        ui_input.right_mouse_down = gs_platform_mouse_down(GS_MOUSE_RBUTTON);
    }
            
    
    // TODO: Handle Input
    if (mouse_x >= 0 && mouse_x <= 720 &&
        mouse_y >= 0 && mouse_y <= 720)
    {
        if (gs_platform_mouse_down(GS_MOUSE_LBUTTON))
        {
            int i = mouse_x / CELL_W; 
            int j = mouse_y / CELL_H;

            state.map[j][i] = CELL_TYPE_obstacle;
        }

        else if (gs_platform_mouse_down(GS_MOUSE_RBUTTON))
        {
            int i = mouse_x / CELL_W; 
            int j = mouse_y / CELL_H;

            state.map[j][i] = CELL_TYPE_open;
        }
    }

    gsi_render_pass_submit(&state.renderer, &state.command_buffer,
                           gs_color(75, 75, 75, 255));

    gsi_camera2D(&state.renderer);

    UIBeginFrame(&state.ui, &state.renderer, &ui_input);
    {
        local_persist ui_id selected = {0};

        ui_id bfs = UIOptionButton(&state.ui, &selected, UIIDGen(), "BFS",
                                   gs_v2(820.f, 100.f), gs_v2(720.f, 0.f),
                                   gs_color(100, 100, 100, 255));
    }

    UIEndFrame(&state.ui);

    // Renderer the map
    for (int j = 0; j < (MAP_H + 1); ++j)
    {
        for (int i = 0; i < (MAP_W + 1); ++i)
        {
            if (i < MAP_W && j < MAP_H)
            {
                if (state.map[j][i] == CELL_TYPE_obstacle)
                {
                    gsi_rectv(&state.renderer,
                            gs_v2(i * CELL_W + CELL_W, j * CELL_H + CELL_H),
                            gs_v2(i * CELL_W, j * CELL_H),
                            GS_COLOR_WHITE, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
                }
            }

            gsi_line(&state.renderer,
                     i * CELL_W, 0, i * CELL_W, 720,
                     255, 255, 255, 255);

            gsi_line(&state.renderer,
                     0, j * CELL_H, 720, j * CELL_H,
                     255, 255, 255, 255);
        }
    }

    gsi_text(&state.renderer, 730.f, 200.f, "Pathfinder",
             NULL, false, 255, 255, 255, 255);

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
