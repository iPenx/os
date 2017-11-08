#ifndef _KERNEL_THREAD_H
#define _KERNEL_THREAD_H

#include "process.h"

typedef void t_thread_func(void*);

struct tcb
{
	uint32_t* kernel_stack;
	unsigned int priority;
	unsigned int ticks;
	unsigned int elapsed_ticks;
	struct pcb* pcb;
	struct doubly_linked_node thread_list_node;
};

extern void init_kernel_thread(void);
extern void start_thread(t_thread_func* func, void* arg);

#endif
