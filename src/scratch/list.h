#ifndef LIST_H
#define LIST_H

typedef struct node
{
    struct node* next;
    cell* data;
} node;

typedef struct linked_list
{
    node *head; 
    node *tail;
    int size;
} linked_list;


#endif