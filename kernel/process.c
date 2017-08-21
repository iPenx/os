#include "process.h"
#include "global.h"
#include "memory.h"
#include "list.h"
#include "thread.h"
#include "debug.h"

void init_kernel_process()
{
    put_str("\ninitial kernel process...");

    struct pcb* pcb=get_mem(sizeof(*pcb));
    pcb->page_dir=KERNEL_PAGE_DIR;
    pcb->page_pool.bitmap.bit=(uint8_t*)KERNEL_PAGE_BITMAP;
    pcb->page_pool.bitmap.num=KERNEL_AVAILABLE_MEM/PAGE_SIZE;
    pcb->page_pool.addr_start=(void*)KERNEL_AVAILABLE_PAGE_START;
    init_kernel_thread(pcb);
}
