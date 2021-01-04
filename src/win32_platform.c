#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include <stdio.h>

#include "win32_platform.h"
#include "math.h"
#include "memory.c"
#include "a_star.c"

global b32 Global_Running;
global platform Global_Platform = {0};

internal void HandleEvent(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_QUIT:
        {
            Global_Running = 0;
        } break;

        case SDL_KEYDOWN: 
        {
            if (event->key.repeat == 0)
            {
                switch (event->key.keysym.scancode)
                {
                    case SDL_SCANCODE_LCTRL:
                    {
                        Global_Platform.ctrl_down = 1;
                    } break;

                    case SDL_SCANCODE_LSHIFT:
                    {
                        Global_Platform.shift_down = 1;
                    } break;

                    case SDL_SCANCODE_SPACE:
                    {
                        Global_Platform.space_down = 1;
                    } break;
                }
            }
        } break;

        case SDL_KEYUP: 
        {
            if (event->key.repeat == 0)
            {
                switch (event->key.keysym.scancode)
                {
                    case SDL_SCANCODE_LCTRL:
                    {
                        Global_Platform.ctrl_down = 0;
                    } break;

                    case SDL_SCANCODE_LSHIFT:
                    {
                        Global_Platform.shift_down = 0;
                    } break;

                    case SDL_SCANCODE_SPACE:
                    {
                        Global_Platform.space_down = 0;
                    } break;
                }
            }
        } break;

        case SDL_MOUSEBUTTONDOWN:
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                Global_Platform.left_mouse_down = 1; 
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                Global_Platform.right_mouse_down = 1;
            }
        } break;

        case SDL_MOUSEBUTTONUP:
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                Global_Platform.left_mouse_down = 0; 
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                Global_Platform.right_mouse_down = 0;
            }
        } break;

        case SDL_MOUSEMOTION:
        {
            SDL_GetMouseState(&Global_Platform.mouse_x, 
                              &Global_Platform.mouse_y);
        } break;
    }
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s", SDL_GetError());
    }

    SDL_Window *window = SDL_CreateWindow("A* Algorithim",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          721, 721, 0);

    if (window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                    SDL_RENDERER_ACCELERATED);
        
        Global_Platform.permanent_storage_size = Megabytes(64); 
        Global_Platform.transient_storage_size = Megabytes(100);

        Global_Platform.permanent_storage = VirtualAlloc(0, Global_Platform.permanent_storage_size,
                                                         MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        Global_Platform.transient_storage = VirtualAlloc(0, Global_Platform.transient_storage_size,
                                                         MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        if (renderer)
        {
            Global_Running = 1;
            while (Global_Running)
            {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    HandleEvent(&event);
                }

                UpdateApp(renderer, &Global_Platform);
            }
        }
        else
        {
            printf("SDL_CreateRenderer failed: %s", SDL_GetError());
        }
    }
    else
    {
        printf("SDL_CreateWindow failed: %s", SDL_GetError());
    }

    SDL_Quit();
    return 0;
}