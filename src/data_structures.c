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
    return ret_list; 
} 

// Pushes node on the beginning of the list
internal void Push(linked_list *list, cell *data)
{
    node *new_node = CreateNode(data, NULL);

    new_node->next = list->head;
    list->head = new_node;
}

// Removes the first element from the list
internal cell *Pop(linked_list *list)
{
    cell *data = list->head->data;
    node *temp = list->head;
    list->head = list->head->next;
    free(temp);
    return data;
}
    
// Returns the data of the top node
internal cell *Top(linked_list *list)
{
    return list->head->data;
}

// Checks if the list is empty
internal b32 Empty(linked_list *list)
{
    return list->head == NULL;
}
