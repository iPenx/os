#include "debug.h"
#include "print.h"
#include "interrupt.h"

unsigned int debug_count=0;

void panic_spin(char* filename, int line, const char* func, const char* condition)
{
    close_intr();
    put_str("\n!!!!! ERROR !!!!!");
    put_str("\nfilename: "); put_str(filename);
    put_str("\nline: "); put_int(line);
    put_str("\nfunction: "); put_str(func);
    put_str("\ncondition: "); put_str(condition);
    while(1);
}

void print_not_complete(const char *filename, int line, const char* func)
{
    put_char('\n');
    put_str(filename);
    put_char('\n');
    put_int(line);
    put_str(": ");
    put_str(func);
    put_str("() is not complete.");
}

