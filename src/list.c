#include "list.h"

// NOTE: The list will not resize, it allocates the 
// max amount of memory that it requires 

internal list CreateList()
{
    list result = {0};
    {
        result.start = 0;
        result.size = 0;
    }

    return result;
}

internal void PushBack(list *l, cell *item)
{
    l->data[l->size++] = item;
}

internal cell *PopFront(list *l) 
{
    cell *result = l->data[l->start];
    l->start++;
    l->size--;
    return result;
}

internal cell *Front(list *l) 
{
    return l->data[l->start];
}

internal b32 Empty(list *l)
{
    return (l->size == 0);
}

// Quick Sort

void Swap(cell *a, cell *b)
{
    cell temp = *a; 
    *a = *b;
    *b = temp;
}

int Partition(cell **arr, int left, int right)
{
    int pivot = arr[right]->f_global_cost;
    int j = (left - 1);

    for (int i = left; i < right; ++i)
    {
        if (arr[i]->f_global_cost <= pivot)
        {
            ++j;
            Swap(arr[j], arr[i]);
        }
    }

    Swap(arr[j + 1], arr[right]);
    return ++j; 
}

void Sort(list *l, int left, int right)
{
    if (l->size < 1)
    {
        cell **arr = l->data;

        if (left < right)
        {
            int partition_index = Partition(arr, left, right);
            Sort(l, left, partition_index - 1);
            Sort(l, partition_index + 1, right);
        }
    }
}
