#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H

#include "bitmap.h"

struct page_pool
{
    struct bitmap bitmap;
    void* addr_start;
};

void* get_a_page(void);
void* get_mem(unsigned int);
void init_memory(void);

#endif
