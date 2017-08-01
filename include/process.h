#ifndef _KERNEL_PROCESS_H
#define _KERNEL_PROCESS_H

#include "stdint.h"
#include "memory.h"
#include "list.h"

struct pcb
{
    unsigned int elapsed_ticks;
    phy_addr_t page_dir;
    struct page_pool page_pool;
    struct doubly_linked_node pcb_node;
    struct doubly_linked_list tcb;
};

void init_kernel_process(void);

#endif

