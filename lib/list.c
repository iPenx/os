#include "list.h"
#include "stdint.h"

int dlist_init(struct doubly_linked_list* dlist)
{
	dlist->head=NULL;
	dlist->tail=NULL;
	return 0;
}

int dlist_append(struct doubly_linked_list* dlist, struct doubly_linked_node* node)
{
	dlist->tail->next=node;
	node->prev=dlist->tail->next;
	node->next=NULL;
	dlist->tail=node;
	return 0;
}

struct doubly_linked_node* dist_get(struct doubly_linked_list* dlist)
{
	struct doubly_linked_node* node;
	node=dlist->head;
	if( node != NULL )
	{
		dlist->head=node->next;
	}
	return node;
}
