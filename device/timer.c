#include "timer.h"
#include "io.h"
#include "stdint.h"
#include "print.h"

#define IRQ0_FREQUENCY 100
#define INPUT_FREQUENCY 1193180
#define COUNTER0_VALUE (INPUT_FREQUENCY / IRQ0_FREQUENCY)
#define PORT_COUNTER0 0x40
#define COUNTER0_NO (0<<6)
#define COUNTER_MODE (2<<1)
#define READ_WRITE_LATCH (3<<4)
#define PORT_PIT_CONTROL 0x43

void init_timer(void)
{
    put_str("\ninitial timer...");
    outb(PORT_PIT_CONTROL, COUNTER0_NO | READ_WRITE_LATCH | COUNTER_MODE);
    outb(PORT_COUNTER0, (uint8_t)COUNTER0_VALUE);
    outb(PORT_COUNTER0, (uint8_t)(COUNTER0_VALUE>>8));
    put_str("\ninitial timer... Done");
}
