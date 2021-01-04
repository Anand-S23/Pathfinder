#include "a_star.h"
#include "renderer.c"

internal b32 CellIsValid(int i, int j)
{
    return (i >= 0 && i < GRID_W && j >= 0 && j < GRID_H);
}

internal void AddNeighbors(app_state *state, cell *cell)
{
    int pos = 0; 

    if (CellIsValid(cell->i, cell->j - 1))
    {
        cell->neighbors[pos] = &state->grid[cell->j - 1][cell->i];
        ++pos;
    }

    if (CellIsValid(cell->i + 1, cell->j - 1))
    {
        cell->neighbors[pos] = &state->grid[cell->j - 1][cell->i + 1];
        ++pos;
    }

    if (CellIsValid(cell->i + 1, cell->j))
    {
        cell->neighbors[pos] = &state->grid[cell->j][cell->i + 1];
        ++pos;
    }

    if (CellIsValid(cell->i + 1, cell->j + 1))
    {
        cell->neighbors[pos] = &state->grid[cell->j + 1][cell->i + 1];
        ++pos;
    }

    if (CellIsValid(cell->i, cell->j + 1))
    {
        cell->neighbors[pos] = &state->grid[cell->j + 1][cell->i];
        ++pos;
    }

    if (CellIsValid(cell->i - 1, cell->j + 1))
    {
        cell->neighbors[pos] = &state->grid[cell->j + 1][cell->i];
        ++pos;
    }

    if (CellIsValid(cell->i - 1, cell->j))
    {
        cell->neighbors[pos] = &state->grid[cell->j][cell->i - 1];
        ++pos;
    }

    if (CellIsValid(cell->i - 1, cell->j - 1))
    {
        cell->neighbors[pos] = &state->grid[cell->j - 1][cell->i - 1];
    }
}

internal void UpdateApp(SDL_Renderer *renderer, platform *platform)
{
    app_state *state = (app_state *)platform->permanent_storage;

    if (!platform->initialized)
    {
        state->permanent_arena = InitMemoryArena(platform->permanent_storage, 
                                                 platform->permanent_storage_size);
        state->transient_arena = InitMemoryArena(platform->transient_storage, 
                                                 platform->transient_storage_size);
        AllocateMemoryArena(&state->permanent_arena, sizeof(app_state));

        for (int j = 0; j < GRID_H; ++j)
        {
            for (int i = 0; i < GRID_W; ++i)
            {
                cell *current_cell = &state->grid[j][i];
                current_cell->i = i; 
                current_cell->j = j;
                current_cell->type = TYPE_walkable;
                current_cell->g_cost = 0.f;
                current_cell->h_cost = 0.f;
                current_cell->parent = NULL;
                AddNeighbors(state, current_cell);
            }
        }

        state->current_mode = MODE_editor;
        state->start = &state->grid[0][0];
        state->end = &state->grid[GRID_H - 1][GRID_W - 1];

        platform->initialized = 1;
    }

    if (state->current_mode == MODE_editor)
    {
        if (platform->ctrl_down)
        {
            if (platform->left_mouse_down)
            {
                int i = platform->mouse_x / CELL_W; 
                int j = platform->mouse_y / CELL_H; 

                if (CellIsValid(i, j))
                {
                    state->start = &state->grid[j][i];
                }
            }
        }
        else if (platform->shift_down)
        {
            if (platform->left_mouse_down)
            {
                int i = platform->mouse_x / CELL_W; 
                int j = platform->mouse_y / CELL_H; 

                if (CellIsValid(i, j))
                {
                    state->end = &state->grid[j][i];
                }
            }
        }
        else if (platform->left_mouse_down)
        {
            int i = platform->mouse_x / CELL_W; 
            int j = platform->mouse_y / CELL_H; 

            if (CellIsValid(i, j))
            {
                state->grid[j][i].type = TYPE_obstacle;
            }
        }
        else if (platform->right_mouse_down)
        {
            int i = platform->mouse_x / CELL_W; 
            int j = platform->mouse_y / CELL_H; 

            if (CellIsValid(i, j))
            {
                state->grid[j][i].type = TYPE_walkable;
            }
        }
    }

    ClearScreen(renderer, v4(0, 0, 0, 255));

    RenderFilledRect(renderer, v4(0, 255, 0, 255),
                     v4(state->start->i * CELL_W, state->start->j * CELL_H, 
                        CELL_W, CELL_H));
    
    RenderFilledRect(renderer, v4(0 , 0, 255, 255),
                     v4(state->end->i * CELL_W, state->end->j * CELL_H, 
                        CELL_W, CELL_H));
    // Render Grid
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); 
    for (int j = 0; j < (GRID_H + 1); ++j)
    {
        for (int i = 0; i < (GRID_W + 1); ++i)
        {
            if (state->grid[j][i].type == TYPE_obstacle)
            {
                RenderFilledRect(renderer, v4(150, 150, 150, 255), 
                                 v4(i * CELL_W, j * CELL_H, CELL_W, CELL_H));
            }

            SDL_RenderDrawLine(renderer,
                               i * CELL_W, 0,
                               i * CELL_W, 720);
            SDL_RenderDrawLine(renderer,
                               0,   j * CELL_H,
                               720, j * CELL_H);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
}