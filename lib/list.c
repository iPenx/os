#include "list.h"
#include "stdint.h"
#include "debug.h"

int dlist_append(struct doubly_linked_list* dlist, struct doubly_linked_node* node)
{
    not_complete();
    return 1;
}

int dlist_init(struct doubly_linked_list* dlist){
    dlist->head.prev=NULL;
    dlist->head.next=&dlist->tail;
    dlist->tail.prev=&dlist->head;
    dlist->tail.next=NULL;
    return 1;
}
