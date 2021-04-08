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
    linked_list list;
    list.head = NULL;
    list.tail = NULL;
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
    
