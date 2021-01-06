#ifndef LIST_H
#define LIST_H

typedef struct list
{
    cell *data[MAX + 1];
    int start;
    int size;
} list;


#endif