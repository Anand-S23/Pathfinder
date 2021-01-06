#include "list.h"

internal node *CreateNode(cell *data)
{
    node *ret_node = (node *)malloc(sizeof(node));
    ret_node->next = NULL;
    ret_node->data = data;
    return ret_node;
}

internal linked_list CreateList()
{
    linked_list ret_list = {0};
    {
        ret_list.head = NULL;
        ret_list.tail = NULL;
        ret_list.size = 0;
    }
    return ret_list;
}

internal void Clear(linked_list *ll)
{
    node *current_node = ll->head;
    ll->tail = NULL;
    ll->size  = 0;

    while (current_node != NULL)
    {
        node *temp = current_node; 
        current_node = current_node->next;
        ll->head = current_node;
        free(temp);
    }
}

internal void Push(linked_list *ll, cell *item)
{
    node *new_node = CreateNode(item);

    if (ll->head == NULL)
    {
        ll->head = new_node;
    }
    else if (ll->tail == NULL)
    {
        ll->tail = ll->head;
        ll->head = new_node;
        ll->head->next = ll->tail;
    }
    else
    {
        new_node->next = ll->head; 
        ll->head = new_node;
    }

    ++ll->size;
}

internal void PushBack(linked_list *ll, cell *item)
{
    node *new_node = CreateNode(item);

    if (ll->head == NULL)
    {
        ll->head = new_node;
    }
    else if (ll->tail == NULL)
    {
        ll->tail = new_node;
        ll->head->next = ll->tail;
    }
    else
    {
        ll->tail->next = new_node; 
        ll->tail = new_node;
    }

    ++ll->size;
}

internal void PopFront(linked_list *ll) 
{
    node *temp = ll->head; 
    ll->head = ll->head->next; 
    free(temp);
    --ll->size;
}

internal cell *Front(linked_list *ll)
{
    return ll->head->data;
}

internal b32 Empty(linked_list *ll)
{
    return (ll->size == 0);
}

// Merge Sort
internal void Split(node *source, node **front_ref, node **back_ref)
{
    node* fast; 
    node* slow; 
    slow = source; 
    fast = source->next; 
  
    while (fast != NULL) 
    { 
        fast = fast->next; 
        if (fast != NULL) 
        { 
            slow = slow->next; 
            fast = fast->next; 
        } 
    } 
  
    *front_ref = source; 
    *back_ref = slow->next; 
    slow->next = NULL; 
}

internal node *SortedMerge(node* a, node* b) 
{ 
    node* result = NULL; 
  
    if (a == NULL) 
    {
        return b; 
    }
    else if (b == NULL) 
    {
        return a; 
    }
  
    if (a->data->f_global_cost <= b->data->f_global_cost) 
    { 
        result = a; 
        result->next = SortedMerge(a->next, b); 
    } 
    else 
    { 
        result = b; 
        result->next = SortedMerge(a, b->next); 
    } 

    return result; 
} 

internal void MergeSort(node **head_ref)
{
    node *head = *head_ref;
    node *a; 
    node *b;

    if (head == NULL || head->next == NULL)
    {
        return;
    }

    Split(head, &a, &b);

    MergeSort(&a);
    MergeSort(&b);

    *head_ref = SortedMerge(a, b);
}

internal void Sort(linked_list *ll)
{
    MergeSort(&ll->head);
}
