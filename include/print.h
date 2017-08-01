#ifndef _LIB_KERNEL_PRINT_H
#define _LIN_KERNEL_PRINT_H
#include "stdint.h"
void cls(void);
void put_char(const uint8_t char_asci);
void put_str(const char * const string);
void put_int(const uint32_t num);
#endif

