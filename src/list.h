#ifndef LIST_H
#define LIST_H

typedef struct list
{
    // TODO: look into metaprograming to implement 
    // generic item for list, so the cell struct
    // does not have to be in file
    int array_position;
    int size; 
} list;

#endif