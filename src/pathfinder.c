#define GS_IMPL
#include <gs.h>

#define GS_IMMEDIATE_DRAW_IMPL
#include <util/gs_idraw.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pathfinder.h"

global app_state state = {0};

void InitApp()
{
    state.command_buffer = gs_command_buffer_new();
    state.renderer = gs_immediate_draw_new();

}

void UpdateApp()
{
    gsi_render_pass_submit(&state.renderer, &state.command_buffer,
                           gs_color(100, 100, 100, 255));

    gsi_camera2D(&state.renderer);

    /*
    for (int j = 0; j < MAP_H; ++j)
    {
        for (int i = 0; i < MAP_W; ++i)
        {
        }
    }
    */

    gsi_rectv(&state.renderer, gs_v2(100.f, 100.f), gs_v2(100.f, 100.f),
              GS_COLOR_WHITE, GS_GRAPHICS_PRIMITIVE_TRIANGLES);

    // Submit command buffer for renderering
    gs_graphics_submit_command_buffer(&state.command_buffer);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    gs_app_desc_t app_desc = {0};
    {
        app_desc.window_title = "Pathfinder";
        app_desc.window_width = 1280;
        app_desc.window_height = 720;
        app_desc.init = InitApp;
        app_desc.update = UpdateApp;
    }

    return app_desc;
}   
