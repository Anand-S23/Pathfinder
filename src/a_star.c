#include <math.h>

#include "a_star.h"
#include "renderer.c"
#include "list.c"

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
        ++pos;
    }

    cell->neighbors_count = pos;
}

internal f32 hueristic(cell *current, cell *end)
{
    // TODO: mess around with this function to see difference in result
    return sqrtf((current->i - end->i)*(current->i - end->i) +
                 (current->j - end->j)*(current->j - end->j));
}

internal void ResetGrid(app_state *state)
{
    for (int j = 0; j < GRID_H; ++j)
    {
        for (int i = 0; i < GRID_W; ++i)
        {
            cell *current_cell = &state->grid[j][i];
            current_cell->type = (current_cell->type == TYPE_obstacle) ? TYPE_obstacle : TYPE_walkable;
            current_cell->f_local_cost = INFINITY;
            current_cell->f_global_cost = INFINITY;
            current_cell->parent = NULL;
        }
    }

    Clear(&state->open);
    state->generating = 0; 
    state->generated = 0;
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
                current_cell->f_local_cost = INFINITY;
                current_cell->f_global_cost = INFINITY;
                current_cell->parent = NULL;
                AddNeighbors(state, current_cell);
            }
        }

        state->current_mode = MODE_menu; 
        state->start = &state->grid[0][0];
        state->end = &state->grid[GRID_H - 1][GRID_W - 1];
        state->open = CreateList();
        state->generating = 0; 
        state->generated = 0;

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
        else if (platform->space_down)
        {
            if (state->generated)
            {
                ResetGrid(state);
            }
            else if (state->current_mode == MODE_editor)
            {
                state->current_mode = MODE_finding;
            }
        }
    }

    // Start path finding if current_mode is MODE_finding
    if (state->current_mode == MODE_finding)
    {
        local_persist cell *current_cell;
        local_persist linked_list *not_tested;

        if (!state->generating)
        {
            current_cell = state->start;
            state->start->f_local_cost = 0.f;
            state->start->f_global_cost = hueristic(state->start, state->end);

            not_tested = &state->open;
            PushBack(not_tested, current_cell);

            state->generating = 1;
        }
        else
        {
            if (!Empty(not_tested) && current_cell != state->end)
            {
                Sort(not_tested);

                // remove all the visited cells from the list
                while (!Empty(not_tested) && 
                    not_tested->head->data->type == TYPE_visited)
                {
                    PopFront(not_tested);
                } 

                if (Empty(not_tested))
                {
                    goto done;
                }

                current_cell = not_tested->head->data;
                current_cell->type = TYPE_visited;

                for (int i = 0; i < current_cell->neighbors_count; ++i)
                {
                    cell *neighbor = current_cell->neighbors[i];
                    if (neighbor->type == TYPE_walkable)
                    {
                        PushBack(not_tested, neighbor);
                    }

                    f32 test_distance = current_cell->f_local_cost + 
                                        hueristic(current_cell, neighbor);

                    if (test_distance < neighbor->f_local_cost)
                    {
                        neighbor->parent = current_cell;
                        neighbor->f_local_cost = test_distance;

                        neighbor->f_global_cost = 
                            neighbor->f_local_cost + hueristic(neighbor, state->end);
                    }
                }
            }
            else
            {
                done:;
                state->generated = 1;
                state->generating = 0;
                state->current_mode = MODE_editor;
            }
        }
    }

    ClearScreen(renderer, v4(0, 0, 0, 255));

    // Fill in the start (green) and end (blue) cells
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
            // The cell is filled if it is an obstacle
            if (state->grid[j][i].type == TYPE_obstacle)
            {
                RenderFilledRect(renderer, v4(150, 150, 150, 255), 
                                 v4(i * CELL_W, j * CELL_H, CELL_W, CELL_H));
            }
            else if (state->grid[j][i].type == TYPE_visited)
            {
                RenderFilledRect(renderer, v4(255, 255, 0, 255), 
                                 v4(i * CELL_W, j * CELL_H, CELL_W, CELL_H));
            }

            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_RenderDrawLine(renderer,
                               i * CELL_W, 0,
                               i * CELL_W, 720);
            SDL_RenderDrawLine(renderer,
                               0,   j * CELL_H,
                               720, j * CELL_H);
        }
    }

    if (state->generated)
    {
        // Render the best path
        if (state->end != NULL)
		{
			cell *current_cell = state->end;
			while (current_cell->parent != NULL)
			{
                RenderFilledRect(renderer, v4(0, 255, 255, 255), 
                                 v4(current_cell->i * CELL_W, current_cell->j * CELL_H, CELL_W, CELL_H));
				
				current_cell = current_cell->parent;
			}
		}

        RenderFilledRect(renderer, v4(0, 255, 255, 255),
                        v4(state->start->i * CELL_W, state->start->j * CELL_H, 
                            CELL_W, CELL_H));
    }

    // Render menu on top if current_mode is menu
    if (state->current_mode == MODE_menu)
    {
        state->current_mode = MODE_editor;
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(50);
}