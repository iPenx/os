#ifndef _KERNEL_GLOBAL_H
#define _KERNEL_GLOBAL_H


#define RPL0 0
#define RPL1 1
#define RPL2 2
#define RPL3 3

#define TI_GDT (0<<2)
#define TI_LDT (1<<2)

#define SELECTOR_CODE ((1<<3) | TI_GDT | RPL0)
#define SELECTOR_DATA ((2<<3) | TI_GDT | RPL0)
#define SELECTOR_VIDEO ((3<<3) | TI_GDT | RPL0)

#define IDT_DESC_P (0x01<<7)
#define IDT_DESC_DPL0 (0x00<<5)
#define IDT_DESC_DPL3 (0x03<<5)
#define IDT_DESC_TYPE_32 0xe
#define IDT_DESC_TYPE_16 0x6

#define IDT_DESC_ATTR_P_DPL0_32 (IDT_DESC_P | IDT_DESC_DPL0 | IDT_DESC_TYPE_32)
#define IDT_DESC_ATTR_P_DPL3_32 (IDT_DESC_P | IDT_DESC_DPL3 | IDT_DESC_TYPE_32)

#define IDT_DESC_39_TO_32_BIT ((0x00<<4) | 0x00)

/***************************************************/
#include "stdint.h"

#define LOADER 0xc0000900

#define PHY_PAGE_BITMAP 0xc001A000
#define AVAILABLE_MEM_START 0xc0100000

#define KERNEL_PAGE_DIR 0xc0090000
#define KERNEL_LOW_4MB_PAGE_TABLE 0xc0091000

#define KERNEL_PAGE_BITMAP 0xc0092000
#define KERNEL_AVAILABLE_PAGE_START 0xc0100000
#define KERNEL_AVAILABLE_MEM 0x40000000

#define PAGE_SIZE 4096

#define KERNEL_TCB 0xc009E000
#define KERNEL_STACK 0xc009F000

struct loader_header
{
    uint32_t size;
    uint32_t entry_16_bits;
    uint32_t total_mem;
};


#endif
