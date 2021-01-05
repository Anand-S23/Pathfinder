#include "list.h"

list CreateList(int alloc_size) /* TODO: also take in type of the items in the list */
{
    list result = {0};

    result.array_position = 0; 
    result.size = 0;

    return result;
}
