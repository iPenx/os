#include "thread.h"
#include "process.h"
#include "global.h"
#include "list.h"
#include "debug.h"

struct doubly_linked_list thread_ready_list;

void init_kernel_thread(struct pcb *pcb)
{
    print_func();
    struct tcb* tcb=(struct tcb*)KERNEL_TCB;
    tcb->kernel_stack=(void*)KERNEL_STACK;
    tcb->priority=32;
    tcb->ticks=32;
    tcb->elapsed_ticks=0;
    dlist_init(&thread_ready_list);
    dlist_append(&thread_ready_list, &tcb->ready_node);
    print_back();
}
