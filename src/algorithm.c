#include "algorithm.h"
/* Common - Utilities functions */

// Checks if the cell is valid
internal b32 CellValid(app_state *state, cell *current, direction direction)
{
    if (state->map[current->j][current->i].walls[direction])
    {
        return 0;
    }

    if (direction == NORTH &&
        state->map[current->j - 1][current->i].type == CELL_TYPE_visited)
    {
        return 0;
    }
    else if (direction == SOUTH &&
             state->map[current->j + 1][current->i].type == CELL_TYPE_visited)
    {
        return 0;
    }
    else if (direction == EAST &&
             state->map[current->j][current->i + 1].type == CELL_TYPE_visited)
    {
        return 0;
    }
    else if (direction == WEST &&
             state->map[current->j][current->i - 1].type == CELL_TYPE_visited)
    {
        return 0;
    }
        
    return 1;
}

// Checks if the two cells passed are the same
internal b32 CellEqual(cell c1, cell c2)
{
    return (c1.i == c2.i && c1.j == c2.j);
}

// Add a cell to neighbor list of current if vaild
internal void AddValidNeighbor(app_state *state, cell *current, direction wall)
{
    if (CellValid(state, current, wall))
    {
        cell neighbor_cell = {0};
        {
            if (wall == NORTH)
            {
                neighbor_cell.j = current->j - 1;
                neighbor_cell.i = current->i;
            }
            else if (wall == SOUTH)
            {
                neighbor_cell.j = current->j + 1;
                neighbor_cell.i = current->i;
            }
            else if (wall == EAST)
            {
                neighbor_cell.j = current->j;
                neighbor_cell.i = current->i + 1;
            }
            else if (wall == WEST)
            {
                neighbor_cell.j = current->j;
                neighbor_cell.i = current->i - 1;
            }
                
            neighbor_cell.parent = current;
            neighbor_cell.neighbor_count = 0;
        }

        current->neighbors[current->neighbor_count] = neighbor_cell;
        ++current->neighbor_count;
    }
}

internal void GeneratePath(app_state *state, linked_list *path_stack)
{
    cell *current = Top(path_stack);
    Pop(path_stack);

    /*
    while (current != NULL)
    {
        state->map[current->j][current->i].type = CELL_TYPE_path;
        free(current->neighbors);
        current->neighbor_count = 0;
        current = Top(path_stack);
        Pop(path_stack);
    }
    */
}


/* Depth First Search */
internal void DFSCleanUp(app_state *state, dfs *dfs, b32 complete)
{
    if (complete)
    {
        Push(&dfs->path_stack, &state->end);
        GeneratePath(state, &dfs->path_stack);
    }

    state->pathfinding = 0; 
    dfs->initialized = 0;
}

internal void DFSPathfinding(app_state *state)
{
    local_persist dfs dfs = {0};

    // Initalize dfs
    if (!dfs.initialized)
    {
        dfs.path_stack = CreateList();
        dfs.current = &state->start;
        dfs.initialized = 1;
    }

    // Add current cell to stack
    Push(&dfs.path_stack, dfs.current);
    state->map[dfs.current->j][dfs.current->i].type = CELL_TYPE_visited;

    if (dfs.current->neighbor_count > 0)
    {
        free(dfs.current->neighbors);
        dfs.current->neighbor_count = 0;
    }

    dfs.current->neighbors = (cell *)malloc(sizeof(cell) * 4);

    AddValidNeighbor(state, dfs.current, NORTH);
    AddValidNeighbor(state, dfs.current, SOUTH);
    AddValidNeighbor(state, dfs.current, EAST);
    AddValidNeighbor(state, dfs.current, WEST);


    // No Valid neighbors left
    if (dfs.current->neighbor_count == 0)
    {
        free(dfs.current->neighbors);
        Pop(&dfs.path_stack);

        if (Empty(&dfs.path_stack))
        {
            state->pathfinding = 0;
        }
        else
        {
            dfs.current = Top(&dfs.path_stack);
            Pop(&dfs.path_stack);
        }
    }
    else
    {
        int rand_element = rand() % (dfs.current->neighbor_count);
        dfs.current = &dfs.current->neighbors[rand_element];
    }

    if (CellEqual(state->end, *dfs.current))
    {
        DFSCleanUp(state, &dfs, 1);
    }
}
