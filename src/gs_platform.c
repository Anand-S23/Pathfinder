#define GS_IMPL
#include <gs.h>

#define GS_IMMEDIATE_DRAW_IMPL
#include <util/gs_idraw.h>

#define GS_ASSET_IMPL
#include <util/gs_asset.h>

#include <windows.h>
#include <stdio.h>

#include "gs_platform.h"
#include "math.h"
#include "memory.c"
#include "app.c"

global platform Global_Platform;

void HandleInput()
{
    Global_Platform.ctrl_down = gs_platform_key_pressed(GS_KEYCODE_CTRL));
    Global_Platform.shift_down = gs_platform_key_pressed(GS_KEYCODE_SHIFT));
    Global_Platform.space_down = gs_platform_key_pressed(GS_KEYCODE_SPACE));

    // TODO: Handle mouse events
}

void InitPlatform() 
{
    Global_Platform = {0};
    {
        Global_Platform.command_buffer = gs_command_buffer_new();
        Global_Platform.immediate_draw = gs_immediate_draw_new();
        Global_Platform.asset_manager  = gs_asset_manager_new();

        Global_Platform.permanent_storage_size = Megabytes(64); 
        Global_Platform.transient_storage_size = Megabytes(100);

        Global_Platform.permanent_storage = VirtualAlloc(0, Global_Platform.permanent_storage_size,
                                                         MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        Global_Platform.transient_storage = VirtualAlloc(0, Global_Platform.transient_storage_size,
                                                         MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    }
}

void PlatformUpdate()
{
    HandleInput();
    // Update App
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    gs_app_desc_t app_desc = {0};
    {
        app_desc.update = UpdatePlatform;
    }

    return app_desc;
}   
