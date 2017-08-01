#ifndef _KERNEL_LIST_H
#define _KERNEL_LIST_H

struct doubly_linked_node
{
    struct doubly_linked_node* prev;
    struct doubly_linked_node* next;
};

struct doubly_linked_list
{
    struct doubly_linked_node head;
    struct doubly_linked_node tail;
};

int dlist_append(struct doubly_linked_list* dlist, struct doubly_linked_node* node);

int dlist_init(struct doubly_linked_list* dlist);

#endif
