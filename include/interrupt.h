#ifndef _KERNEL_ITERRUPT_H
#define _KERNEL_ITERRUPT_H

typedef void* intr_handler;
void init_interrupt(void);

enum status_interrupt
{
    INTR_OFF,
    INTR_ON
};

enum status_interrupt get_intr_status(void);
enum status_interrupt set_intr_status(enum status_interrupt status);
enum status_interrupt open_intr(void);
enum status_interrupt close_intr(void);

#endif
