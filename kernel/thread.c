#include "thread.h"
#include "process.h"
#include "global.h"
#include "list.h"
#include "interrupt.h"
#include "string.h"
#include "debug.h"

extern void switch_to(struct tcb *cur, struct tcb *next);
#define offset(struct, struct_member) (int)(&((struct*)0)->struct_member)
#define elem_to_entry(struct, struct_member, elem_ptr) (struct *)((int)elem_ptr-offset(struct, struct_member))

static struct doubly_linked_list thread_ready_list;
static struct tcb* cur_thread;

static struct tcb* get_next_thread(void);
static void scheduler_next_one(void);
static void schedule(int intr_num);
static void install_scheduler(void);

void create_thread(t_thread new_thread, void *arg)
{
	//create tcb for new_thread
	struct tcb *tcb=get_pages(1);
	tcb->kernel_stack=(int)tcb+PAGE_SIZE-sizeof(struct kernel_thread_launch_stack;
	tcb->priority=DEFAULT_PRIORITY;
	tcb->ticks=DEFAULT_PRIORITY;

	//prepare thread env
	struct kernel_thread_launch_stack *kernel_stack=tcb->kernel_stack;
	kernel_stack->

	//add new_thread tcb to thread_ready_list
}

void init_kernel_thread(struct pcb *pcb)
{
    struct tcb* tcb=(struct tcb*)KERNEL_TCB;
    tcb->kernel_stack=(void*)KERNEL_STACK;
    tcb->priority=32;
    tcb->ticks=32;
    tcb->elapsed_ticks=0;
    dlist_init(&thread_ready_list);
    cur_thread=tcb;
    install_scheduler();
}

void scheduler_next_one()
{
	put_str("\nscheduler: next thread.");
	struct tcb* next;
	next=get_next_thread();

	struct tcb* cur;
	cur=cur_thread;

	cur_thread=next;

	switch_to(cur, next);
}

void schedule(int intr_num)
{
	if(cur_thread->ticks == 0)
	{
		cur_thread->ticks=cur_thread->priority;
		dlist_append(&thread_ready_list, &cur_thread->thread_list_node);
		scheduler_next_one();
	}
	else
	{
		(cur_thread->ticks)--;
	}
}

void install_scheduler(void)
{
	register_intr(INTR_TIMER, schedule);
}

struct tcb* get_next_thread(void)
{
	struct doubly_linked_node* node=dlist_pop(&thread_ready_list);
	struct tcb* next=elem_to_entry(struct tcb, thread_list_node, node);
	return next; 
}

