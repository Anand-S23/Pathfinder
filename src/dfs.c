internal b32 CellValid(cell_type type, int x, int y)
{
    return (type == CELL_TYPE_open &&
            (x >= 0 && x < MAP_W) &&
            (y >= 0 && y < MAP_H));
}

internal b32 CellEqual(cell c1, cell c2)
{
    return (c1.i == c2.i && c1.j == c2.j);
}

internal void DFSPathfinding(app_state *state)
{
    if (state->generating)
    {
        local_persist b32 initialized = 0;
        local_persist linked_list path_stack;
        local_persist cell *current;

        if (!initalized)
        {
            path_stack = CreateList();
            current = state->start;
            initialized = 1;
        }

        Push(&path_stack, current);
        current->type = CELL_TYPE_visited;

        int size = 0;

        // North
        if (CellValid(current->type, current->i, current->j - 1))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j - 1][current->i]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j - 1][current->i];
            }
        }

        // North East
        if (CellValid(current->type, current->i + 1, current->j - 1))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j - 1][current->i + 1]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j - 1][current->i + 1];
            }
        }

        // East
        if (CellValid(current->type, current->i + 1, current->j))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j][current->i + 1]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j][current->i + 1];
            }
        }

        // South East
        if (CellValid(current->type, current->i + 1, current->j + 1))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j + 1][current->i + 1]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j + 1][current->i + 1];
            }
        }

        // South
        if (CellValid(current->type, current->i, current->j + 1))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j + 1][current->i]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j + 1][current->i];
            }
        }

        // South West
        if (CellValid(current->type, current->i - 1, current->j + 1))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j + 1][current->i - 1]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j + 1][current->i - 1];
            }
        }

        // West
        if (CellValid(current->type, current->i - 1, current->j))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j][current->i - 1]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j][current->i - 1];
            }
        }

        // North West
        if (CellValid(current->type, current->i - 1, current->j - 1))
        {
            b32 found = 0;

            if (current->parent != NULL)
            {
                for (int i = 0; i < ArraySize(current->parent->neighbors); ++i)
                {
                    if (CellEqual(current->parent->neighbors[i],
                                  state->map[current->j - 1][current->i - 1]))
                    {
                        found = 1;
                        break;
                    }
                }
            }
                            
            if (!found)
            {
                current->neighbors[size++] =
                    state->map[current->j - 1][current->i - 1];
            }
        }

        if (size == 0)
        {
            // No Valid neighbors left
            Pop(&path_stack);

            if (Empty(&path_stack))
            {
                // TODO: Handle when there is no possible path
            }
            else
            {
                current = Top(&path_stack);
            }
        }
        else
        {
            // TODO: Choose random element to choose as current
        }

        // TODO: Check if the new current is the end
    }
}
