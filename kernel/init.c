#include "init.h"
#include "print.h"
#include "memory.h"
#include "interrupt.h"
#include "timer.h"
#include "process.h"

void init_all(void)
{
    put_str("\nInitialization");
    init_memory();
    init_kernel_process();
    init_interrupt();
    init_timer();
}
