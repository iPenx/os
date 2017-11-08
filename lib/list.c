#include "list.h"
#include "stdint.h"

int dlist_init(struct doubly_linked_list* dlist)
{
	struct doubly_linked_node *head, *tail;
	head=&(dlist->head);
	tail=&(dlist->tail);
	head->prev=NULL;
	head->next=tail;
	tail->prev=head;
	tail->next=NULL;
	return 0;
}

int dlist_add(struct doubly_linked_list* dlist, struct doubly_linked_node* node)
{
	struct doubly_linked_node *prev, *next;
	prev=dlist->tail.prev;
	next=&(dlist->tail);
	prev->next=node;
	node->prev=prev;
	node->next=next;
	next->prev=node;
	return 0;
}

struct doubly_linked_node* dlist_pop(struct doubly_linked_list* dlist)
{
	struct doubly_linked_node *node, *head;
	head=&(dlist->head);
	node=head->next;
	if( node == &(dlist->tail) )
	{
		return NULL;
	}
	else
	{
		head->next=node->next;
		head->next->prev=head;
		return node;
	}
}

int dlist_push(struct doubly_linked_list* dlist, struct doubly_linked_node* node)
{
	struct doubly_linked_node *prev, *next;
	prev=&(dlist->head);
	next=prev->next;
	prev->next=node;
	node->prev=prev;
	node->next=next;
	next->prev=node;
	return 0;
}

