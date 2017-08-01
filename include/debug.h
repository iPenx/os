#ifndef _KERNEL_DEBUG_H
#define _KERNEL_DEBUG_H

#include "stdint.h"
#include "print.h"

void panic_spin(char* filename, int line, const char* func, const char* condition);
void print_not_complete(const char* filename, int line, const char* func);

#define PANIC(...) panic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
    #define ASSERT(CONDITION)
    #define print_func()
    #define print_back()
    #define not_intall()
#else
extern unsigned int debug_count;
    #define ASSERT(CONDITION) \
            if(!(CONDITION)) \
	        PANIC(#CONDITION);

    #define print_func() unsigned int debug_func_count=++debug_count; \
			      put_char('\n'); put_int(debug_func_count); put_char(' '); put_str(__func__); put_str("()")

    #define print_back()  put_char('\n'); put_int(debug_func_count); put_str(" back!")
    #define not_complete()  print_not_complete(__FILE__, __LINE__-2, __func__)
#endif

#endif
