#include "data_structures.h"

/* Linked List */

// Makes a new node using malloc
internal node *CreateNode(cell *data, node* next)
{
    node *ret_node = (node *)malloc(sizeof(node));
    ret_node->data = data;
    ret_node->next = next;
    return ret_node;
}

// Makes a new empty list
internal linked_list CreateList()
{
    linked_list list = {0};
    list.head = NULL;
    list.tail = NULL;
    list.len = 0;
    return list; 
} 

// Pushes node on the beginning of the list
internal void Push(linked_list *list, cell *data)
{
    node *new_node = CreateNode(data, NULL);
    new_node->next = list->head;
    list->head = new_node;

    if (list->tail == NULL)
    {
        list->tail = list->head->next;
    }

    ++list->len;
}

// Appends node to the end of the list
internal void Append(linked_list *list, cell *data)
{
    node *new_node = CreateNode(data, NULL);

    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else if (list->tail == NULL)
    {
        list->tail = new_node;
        list->head->next = list->tail;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = list->tail->next;
    }

    ++list->len;
}

internal void MinSortedInsert(linked_list *list, cell *data)
{
    node *new_node = CreateNode(data, NULL);

    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else if (data->dist < list->head->data->dist)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        node *current = list->head;
        b32 inserted = 0;

        for (int i = 0; i < list->len - 1; ++i)
        {
            if (data->dist < current->next->data->dist)
            {
                new_node->next = current->next;
                current->next = new_node;
                inserted = 1;
                break;
            }
        }

        if (!inserted)
        {
            if (list->tail == NULL)
            {
                list->tail = new_node;
                list->head->next = list->tail;
            }
            else
            {
                list->tail->next = new_node;
                list->tail = new_node;
            }
        }
    }

    ++list->len;
}

internal void MaxSortedInsert(linked_list *list, cell *data)
{
    node *new_node = CreateNode(data, NULL);

    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else if (data->dist > list->head->data->dist)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        node *current = list->head;
        b32 inserted = 0;

        for (int i = 0; i < list->len - 1; ++i)
        {
            if (data->dist > current->next->data->dist)
            {
                new_node->next = current->next;
                current->next = new_node;
                inserted = 1;
                break;
            }
        }

        if (!inserted)
        {
            if (list->tail == NULL)
            {
                list->tail = new_node;
                list->head->next = list->tail;
            }
            else
            {
                list->tail->next = new_node;
                list->tail = new_node;
            }
        }
    }

    ++list->len;
}

// Removes the first element from the list
internal void Pop(linked_list *list)
{
    node *temp = list->head;
    list->head = list->head->next;
    free(temp);

    if (list->tail != NULL)
    {
        if (list->head->next == NULL)
        {
            list->tail = NULL;
        }
    }

    --list->len;
}
    
// Returns the data of the top node
internal cell *Top(linked_list *list)
{
    return list->head->data;
}

internal cell *Bottom(linked_list *list)
{
    return list->tail->data;
}

// Checks if the list is empty
internal b32 Empty(linked_list *list)
{
    return list->head == NULL;
}

internal void PrintList(linked_list *list)
{
    node *current = list->head;
    int len = 0;

    while (current != NULL)
    {
        // printf("(%d, %d) ", current->data->j, current->data->i);
        current = current->next;
        ++len;
    }

    printf("\n---%d---\n", len);
}
    
