#ifndef _KERNEL_TASK_H
#define _KERNEL_TASK_H

#include "stdint.h"
#include "list.h"

enum task_status
{
    RUNNING,
    READY,
    BLOCKED,
    WAITING,
    HANGING,
    DIED
};


struct tcb
{
    uint32_t stack;
    enum task_status status;
    char name[18];
    uint8_t priority;
    uint8_t ticks;
    uint32_t elapsed_ticks;
    struct bi_directional_list node_ready;
    struct bi_directiaonl_list node_all;
    uint32_t page_pool;
}

#define get_tcb(tcb) ( \
                         asm("movd %%esp, %0":"=g"(tcb)); \
                         tcb&=0xfffff000; \
                     )

int init_kernel_process(void);

#endif
