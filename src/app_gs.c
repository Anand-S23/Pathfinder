#include "app.h"

internal void UpdateApp(platform *platform)
{
    app_state *state = (app_state *)platform->permanent_storage;

    if (!platform->initialized)
    {
        state->permanent_arena = InitMemoryArena(platform->permanent_storage, 
                                                 platform->permanent_storage_size);
        state->transient_arena = InitMemoryArena(platform->transient_storage, 
                                                 platform->transient_storage_size);
        AllocateMemoryArena(&state->permanent_arena, sizeof(app_state));
    }


    // Clear Screen
    gsi_render_pass_submit(&platform->immediate_draw,
                           &platform->command_buffer,
                           gs_color(100, 100, 100, 255));

    gsi_camera2D(&platform->immediate_draw);

    gsi_rectv(&platform->immediate_draw, gs_v2(100.f, 100.f), gs_v2(100.f, 100.f),
              GS_COLOR_WHITE, GS_GRAPHICS_PRIMITIVE_TRIANGLES);

    // Submit command buffer for renderering
    gs_graphics_submit_command_buffer(&platform->command_buffer);
}
