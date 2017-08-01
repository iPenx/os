#include "print.h"
#include "init.h"
#include "debug.h"
#include "bitmap.h"

void main(void)
{
    cls();
    put_str("kernel has been loaded.");
    init_all();
    while(1);
}
