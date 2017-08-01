#include "interrupt.h"
#include "global.h"
#include "stdint.h"
#include "print.h"
#include "io.h"


struct gate_desc
{
    uint16_t func_offset_low_word;
    uint16_t selector;
    uint8_t bit_39_32;
    uint8_t attribute;
    uint16_t func_offset_high_word;
};

#define INTERRUPT_DESC_COUNT 0x21
extern intr_handler iet[INTERRUPT_DESC_COUNT];

static void interrupt_desc_init(void);
static void pic_init(void);
static void handle_interrupt(uint8_t vector);

void* intr_proc_table[INTERRUPT_DESC_COUNT]=
{
    /*0*/ handle_interrupt,
    /*1*/ handle_interrupt,
    /*2*/ handle_interrupt,
    /*3*/ handle_interrupt,
    /*4*/ handle_interrupt,
    /*5*/ handle_interrupt,
    /*6*/ handle_interrupt,
    /*7*/ handle_interrupt,
    /*8*/ handle_interrupt,
    /*9*/ handle_interrupt,
    /*10*/ handle_interrupt,
    /*11*/ handle_interrupt,
    /*12*/ handle_interrupt,
    /*13*/ handle_interrupt,
    /*14*/ handle_interrupt,
    /*15*/ handle_interrupt,
    /*16*/ handle_interrupt,
    /*17*/ handle_interrupt,
    /*18*/ handle_interrupt,
    /*19*/ handle_interrupt,
    /*20*/ handle_interrupt,
    /*21*/ handle_interrupt,
    /*22*/ handle_interrupt,
    /*23*/ handle_interrupt,
    /*24*/ handle_interrupt,
    /*25*/ handle_interrupt,
    /*26*/ handle_interrupt,
    /*27*/ handle_interrupt,
    /*28*/ handle_interrupt,
    /*29*/ handle_interrupt,
    /*30*/ handle_interrupt,
    /*31*/ handle_interrupt,
    /*32*/ handle_interrupt
};

static struct gate_desc idt[INTERRUPT_DESC_COUNT];

static char *intr_name[INTERRUPT_DESC_COUNT]=
{
    /*0*/ "#DE Divide Error",
    /*1*/ "#DB Debug Exception",
    /*2*/ "#NMI Nonmask Interrupt",
    /*3*/ "#BP Breakpoint Exception",
    /*4*/ "#OF Overflow Exception",
    /*5*/ "#BR Bound Range Exceeded Exception",
    /*6*/ "#UD Invalid Opcode Exception",
    /*7*/ "#NM Device Not Available Exception",
    /*8*/ "DF Double Fault Exception",
    /*9*/ "Coprocessor Segment Overrun",
    /*10*/ "#TS Invalid TSS Exception",
    /*11*/ "#NP Segment Not Present",
    /*12*/ "#SS Stack Fault Exception",
    /*13*/ "#GP General Protection Exception",
    /*14*/ "#PF Page-Fault Exception",
    /*15*/ "Reserved",
    /*16*/ "#MF x87 FPU Floating-Point Error",
    /*17*/ "#AC Alignment Check Exception",
    /*18*/ "#MC Machine-Check Exception",
    /*19*/ "#XF SIMD Floating-Point Exception",
    /*20*/ "Reserved",
    /*21*/ "Reserved",
    /*22*/ "Reserved",
    /*23*/ "Reserved",
    /*24*/ "Reserved",
    /*25*/ "Reserved",
    /*26*/ "Reserved",
    /*27*/ "Reserved",
    /*28*/ "Reserved",
    /*29*/ "Reserved",
    /*30*/ "Reserved",
    /*31*/ "Reserved",
};

static void make_intr_desc(struct gate_desc *p_gdesc, uint8_t attr, intr_handler function)
{
    p_gdesc->func_offset_low_word=(uint32_t)function & 0xffff;
    p_gdesc->selector=SELECTOR_CODE;
    p_gdesc->bit_39_32=IDT_DESC_39_TO_32_BIT;
    p_gdesc->attribute=attr;
    p_gdesc->func_offset_high_word=((uint32_t)function & 0xFFFF0000)>>16;
}

static void install_intr_desc(void)
{
    int i;
    for(i=0;i<INTERRUPT_DESC_COUNT;i++)
        make_intr_desc(&idt[i], IDT_DESC_ATTR_P_DPL0_32, iet[i]);
}

#define PORT_PIC_MASTER_CTRL 0x20
#define PORT_PIC_MASTER_DATA 0x21
#define PORT_PIC_SLAVE_CTRL 0xa0
#define PORT_PIC_SLAVE_DATA 0xa1

static void init_pic(void)
{
    outb(PORT_PIC_MASTER_CTRL, 0x11);
    outb(PORT_PIC_MASTER_DATA, 0x20);

    outb(PORT_PIC_MASTER_DATA, 0x04);
    outb(PORT_PIC_MASTER_DATA, 0x01);

    outb(PORT_PIC_SLAVE_CTRL, 0x11);
    outb(PORT_PIC_SLAVE_DATA, 0x28);

    outb(PORT_PIC_SLAVE_DATA, 0x02);
    outb(PORT_PIC_SLAVE_DATA, 0x01);

    outb(PORT_PIC_MASTER_DATA, 0xfe);
    outb(PORT_PIC_SLAVE_DATA, 0xff);
}

static void handle_interrupt(uint8_t vector)
{
    if(vector==0x27 || vector==0x2f)
        return;
    put_str("\nint 0x"); put_int(vector);
}

void init_interrupt(void)
{
    put_str("\ninitial interrupt...");
    install_intr_desc();
    init_pic();
    
    uint64_t idt_operand=((sizeof(idt)-1) | ((uint64_t)((uint32_t)idt<<16)));
    asm volatile("lidt %0"::"m"(idt_operand));
    put_str("\ninitial interrupt... Done");
}

#define EFLAGS_IF 0x00000200
#define get_EFLAGS(VAR_EFLAGS) asm volatile("pushfl; \
                                             popl %0" \
					     : "=g"(VAR_EFLAGS))

enum status_interrupt open_intr()
{
    enum status_interrupt old_status;
    old_status=get_intr_status();
    asm volatile("sti");
    return old_status;
}

enum status_interrupt close_intr()
{
    enum status_interrupt old_status;
    old_status=get_intr_status();
    asm volatile("cli");
    return old_status;
}

enum status_interrupt set_intr_status(enum status_interrupt status)
{
    return (status == INTR_ON)? open_intr() : close_intr();
}

enum status_interrupt get_intr_status()
{
    uint32_t eflags;
    get_EFLAGS(eflags); return (EFLAGS_IF & eflags)? INTR_ON : INTR_OFF; }
