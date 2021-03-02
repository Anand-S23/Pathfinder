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
    if (CellValid(state, current, direction)
    {
        cell neighbor_cell = {0};
        {
            neighbor_cell.j = j;
            neighbor_cell.i = i;
            neighbor_cell.parent = current;
            neighbor_cell.neighors = (cell *)malloc(sizeof(cell) * 4);
            neighbor_count = 0;
        }

        b32 found = 0;

        // Check if neighbor cell is child of current's parent cell
        if (current->parent != NULL)
        {
            for (int i = 0; i < current->parent->neighbor_count; ++i)
            {
                if (CellEqual(current->parent->neighbors[i],
                              neighbor_cell))
                {
                    found = 1;
                    break;
                }
            }
        }

        if (!found)
        {
            current->neighbors[current->neighbor_count++] = neighbor_cell;
        }
    }
}

internal void GeneratePath(app_state *state, Linked_List *path_stack)
{
    cell *current = Pop(path_stack);

    while (current != NULL)
    {
        state->map[current->j][current->i] = CELL_TYPE_path;
        current = Pop(path_stack);
    }
}


/* Depth First Search */
internal void DFSPathfinding(app_state *state)
{
    local_persist dfs dfs = {0};

    // Initalize dfs
    if (!dfs.initalized)
    {
        dfs.path_stack = CreateList();
        dfs.current = state->start;
        dfs.initialized = 1;
    }

    // Add current cell to stack
    Push(&dfs.path_stack, dfs.current);
    dfs.current->type = CELL_TYPE_visited;

    AddValidNeighbor(state, dfs.current, NORTH);
    AddValidNeighbor(state, dfs.current, SOUTH);
    AddValidNeighbor(state, dfs.current, EAST);
    AddValidNeighbor(state, dfs.current, WEST);

    // No Valid neighbors left
    if (dfs.current->neighbor_count == 0)
    {
        Pop(&dfs.path_stack);

        if (Empty(&dfs.path_stack))
        {
            state->pathfinding = 0;
        }
        else
        {
            current = Top(&dfs.path_stack);
        }
    }
    else
    {
        int rand_element = rand() % (dfs.current->neighbor_count + 1);
        dfs.current = dfs.current->neighbors[rand_element];
    }

    if (CellEqual(state->end, *dfs.current))
    {
        Push(&dfs.path_stack, &state->end);
        GeneratePath(state, &dfs.path_stack);
        state->pathfinding = 0; 
    }
}
