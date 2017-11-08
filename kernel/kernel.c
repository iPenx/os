#include "print.h"
#include "init.h"
#include "debug.h"
#include "bitmap.h"
#include "thread.h"
#include "interrupt.h"

static void thread_one(void* str);
static void thread_two(void* num);

void main(void)
{
	cls();
	put_str("kernel has been loaded.");
	init_all();
	open_intr();
	int num=10;
	start_thread(thread_one, "thread_one");
	start_thread(thread_two, &num);
	while(1)
	{
		int old_intr;
		old_intr=close_intr();
		put_str("\nkernel: test...");
		set_intr_status(old_intr);
	}
}

void thread_one(void* str)
{
	while(1)
	{
		int old_intr;
		old_intr=close_intr();
		put_str("\n");
		put_str(str);
		put_str(": Hello!");
		set_intr_status(old_intr);
	}
}

void thread_two(void* num)
{
	while(1)
	{
		int old_intr;
		old_intr=close_intr();
		put_str("\nthread_two: I got a arg, ");
		put_int(*(int*)num);
		set_intr_status(old_intr);
	}
}
