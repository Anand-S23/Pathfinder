#include "a_star.h"
#include "renderer.c"

internal void UpdateApp(SDL_Renderer *renderer, platform *platform)
{
    game_state *state = (game_state *)platform->permanent_storage;

    if (!platform->initialized)
    {
        state->permanent_arena = InitMemoryArena(platform->permanent_storage, 
                                                 platform->permanent_storage_size);
        state->transient_arena = InitMemoryArena(platform->transient_storage, 
                                                 platform->transient_storage_size);
        AllocateMemoryArena(&state->permanent_arena, sizeof(game_state));

        platform->initialized = 1;
    }

    ClearScreen(renderer, v4(0, 0, 0, 255));

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}