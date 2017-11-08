#include "thread.h"
#include "process.h"
#include "global.h"
#include "list.h"
#include "interrupt.h"
#include "string.h"
#include "debug.h"

#define DEFAULT_PRIORITY 32

struct s_thread_stack
{
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edi;
	uint32_t esi;
	void (*eip)(t_thread_func* func, void* arg);
	void* non_return;
	t_thread_func* thread;
	void* arg;
};
	
extern void switch_to(struct tcb *cur, struct tcb *next);
#define offset(struct, struct_member) (int)(&((struct*)0)->struct_member)
#define elem_to_entry(struct, struct_member, elem_ptr) (struct *)((int)elem_ptr-offset(struct, struct_member))

static struct doubly_linked_list thread_ready_list;
static struct tcb* cur_thread;

static struct tcb* get_next_thread(void);
static void scheduler_next_one(void);
static void schedule(int intr_num);
static void install_scheduler(void);
static void laucher(t_thread_func* func, void* arg);

void start_thread(t_thread_func new_thread, void *arg)
{
	//create tcb for new_thread
	struct tcb *tcb=get_a_page();
	tcb->kernel_stack=(uint32_t *)((uint32_t)tcb+PAGE_SIZE-sizeof(struct s_thread_stack));
	tcb->priority=DEFAULT_PRIORITY;
	tcb->ticks=DEFAULT_PRIORITY;

	//prepare thread env
	struct s_thread_stack *thread_stack=tcb->kernel_stack;
	thread_stack->eip=laucher;
	thread_stack->thread=new_thread;
	thread_stack->arg=arg;

	//add new_thread tcb to thread_ready_list
	dlist_add(&thread_ready_list, &tcb->thread_list_node);
}

static void laucher(t_thread_func* func, void* arg)
{
	put_str("\nlauch a new thread");
	open_intr();
	func(arg);
}

void init_kernel_thread()
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
	put_int(next);

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
		dlist_add(&thread_ready_list, &cur_thread->thread_list_node);
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

