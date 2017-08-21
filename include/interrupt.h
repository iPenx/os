#ifndef _KERNEL_ITERRUPT_H
#define _KERNEL_ITERRUPT_H

typedef void* intr_handler;
typedef void (*t_intr_proc)(int);

enum status_interrupt
{
	INTR_OFF,
	INTR_ON
};

enum intr_name
{
	INTR_TIMER=0x20
};

extern void init_interrupt(void);
extern enum status_interrupt get_intr_status(void);
extern enum status_interrupt set_intr_status(enum status_interrupt status);
extern enum status_interrupt open_intr(void);
extern enum status_interrupt close_intr(void);
extern int register_intr(enum intr_name intr_name, t_intr_proc proc);

#endif
