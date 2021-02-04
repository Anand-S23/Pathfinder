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
#include "app_gs.c"

global platform Global_Platform = {0};

void HandleInput()
{
    // TODO: Check to see this is the most optimal way to handle input
    // Handle Keyboard Input
    Global_Platform.ctrl_down  = gs_platform_key_down(GS_KEYCODE_LCTRL);
    Global_Platform.shift_down = gs_platform_key_down(GS_KEYCODE_LSHIFT);
    Global_Platform.space_down = gs_platform_key_down(GS_KEYCODE_SPACE);

    // Mouse Input 
    gs_platform_mouse_position(&Global_Platform.mouse_x, &Global_Platform.mouse_y);
    Global_Platform.left_mouse_down = gs_platform_mouse_down(GS_MOUSE_LBUTTON);
    Global_Platform.right_mouse_down = gs_platform_mouse_down(GS_MOUSE_RBUTTON);
}

void InitPlatform() 
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

void UpdatePlatform()
{
    HandleInput();
    UpdateApp(&Global_Platform);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    gs_app_desc_t app_desc = {0};
    {
        app_desc.window_title = "Pathfinder";
        app_desc.window_width = 1280;
        app_desc.window_height = 720;
        app_desc.init = InitPlatform;
        app_desc.update = UpdatePlatform;
    }

    return app_desc;
}   
