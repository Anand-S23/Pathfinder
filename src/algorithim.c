/* Common - Utilities functions */

// Checks if cell taversable (open and in map)
internal b32 CellValid(cell_type type, int x, int y)
{
    return (type == CELL_TYPE_open &&
            (x >= 0 && x < MAP_W) &&
            (y >= 0 && y < MAP_H));
}

// Checks if the two cells passed are the same
internal b32 CellEqual(cell c1, cell c2)
{
    return (c1.i == c2.i && c1.j == c2.j);
}

// Add a cell to neighbor list of current if vaild
internal void AddValidNeighbor(cell *current, int i, int j)
{
    if (CellValid(current->type, i, j))
    {
        cell neighbor_cell = {0};
        {
            neighbor_cell.j = j;
            neighbor_cell.i = i;
            neighbor_cell.parent = current;
            neighbor_cell.neighors = malloc(sizeof(cell) * 4);
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

    AddValidNeighbor(dfs.current, dfs.current->i, dfs.current->j - 1);     // North
    AddValidNeighbor(dfs.current, dfs.current->i + 1, dfs.current->j - 1); // North East
    AddValidNeighbor(dfs.current, dfs.current->i + 1, dfs.current->j);     // East
    AddValidNeighbor(dfs.current, dfs.current->i + 1, dfs.current->j + 1); // South East
    AddValidNeighbor(dfs.current, dfs.current->i, dfs.current->j + 1);     // South
    AddValidNeighbor(dfs.current, dfs.current->i - 1, dfs.current->j + 1); // South West
    AddValidNeighbor(dfs.current, dfs.current->i - 1, dfs.current->j);     // West
    AddValidNeighbor(dfs.current, dfs.current->i - 1, dfs.current->j - 1); // North West

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
