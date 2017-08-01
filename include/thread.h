#ifndef _KERNEL_THREAD_H
#define _KERNEL_THREAD_H

#include "process.h"

struct tcb
{
    void* kernel_stack;
    unsigned int priority;
    unsigned int ticks;
    unsigned int elapsed_ticks;
    struct pcb* pcb;
    struct doubly_linked_node ready_node;
};

void init_kernel_thread(struct pcb *pcb);

#endif
