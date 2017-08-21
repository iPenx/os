#include "print.h"
#include "init.h"
#include "debug.h"
#include "bitmap.h"

void main(void)
{
	cls();
	put_str("kernel has been loaded.");
	init_all();
	open_intr();
	while(1)
	{
		int old_intr;
		old_intr=close_intr();
		put_str("\nkernel: test...");
		set_intr_status(old_intr);
	}
}
