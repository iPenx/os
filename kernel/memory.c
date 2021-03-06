#include "memory.h"
#include "global.h"
#include "x86.h"
#include "process.h"
#include "bitmap.h"
#include "stdint.h"
#include "string.h"
#include "print.h"
#include "debug.h"

struct phy_page_pool
{
	struct bitmap bitmap;
	phy_addr_t addr_start;
};

#define get_pte(addr) (uint32_t*)(0xffc00000 | (((uint32_t)addr&0xffc00000)>>10) | (((uint32_t)addr&0x003ff000)>>10))

#define get_pde(addr) (uint32_t*)(0xfffff000 | (((uint32_t)addr&0xffc00000)>>20))

static phy_addr_t get_a_phy_page(void);
static int mapping_addr(void* addr);
static void* get_pages(unsigned int num);
static int check_addr_mapping(void* addr);

static struct phy_page_pool phy_page_pool;
static struct page_pool kernel_page_pool;

void init_memory()
{
	put_str("\ninital memory...");

	phy_page_pool.addr_start=AVAILABLE_MEM_START;
	uint32_t total_mem=((struct loader_header*)(LOADER))->total_mem;
	struct bitmap* bitmap=&phy_page_pool.bitmap;
	bitmap->bit=(uint8_t*)PHY_PAGE_BITMAP;
	bitmap->num=total_mem/PAGE_SIZE;
	memset(bitmap->bit, 0, (bitmap->num)>>3);

	kernel_page_pool.addr_start=(void*)KERNEL_AVAILABLE_PAGE_START;
	bitmap=&kernel_page_pool.bitmap;
	bitmap->bit=(uint8_t*)KERNEL_PAGE_BITMAP;
	bitmap->num=KERNEL_AVAILABLE_MEM/PAGE_SIZE;
	memset(bitmap->bit, 0, (bitmap->num)>>3);

	put_str("\ninital memory... Done");
}   

static phy_addr_t get_a_phy_page()
{
	unsigned int index;
	if( (index=bitmap_get_one(&phy_page_pool.bitmap)) != BITMAP_FULL)
	{
		return (phy_page_pool.addr_start+index*PAGE_SIZE);
	}
	return 0;
}

static int mapping_addr(void* addr)
{
	uint32_t* pde=get_pde(addr);
	uint32_t* pte=get_pte(addr);
	phy_addr_t phy_addr;
	if( !(*pde&PDE_P) )
	{
		if( (phy_addr=get_a_phy_page()) != 0)
		{
			*pde=(phy_addr | PDE_U | PDE_W | PDE_P);
			memset((void*)((uint32_t)pte&0xfffff000), 0, PAGE_SIZE);
		}
		else
		{
			return 1;
		}
	}
	if( !(*pte&PTE_P) )
	{
		if( (phy_addr=get_a_phy_page()) != 0)
		{
			put_int(phy_addr);
			*pte=(phy_addr | PDE_U | PDE_W | PDE_P);
		}
	else
	{
		return 0;
		}
	}
	return 0;
}

static int check_addr_mapping(void* addr)
{
	print_func();
	uint32_t* pde=get_pde(addr);
	uint32_t* pte=get_pte(addr);
	
	if( !(*pde&PDE_P) )
	{
		print_back();
		return 0;
	}
	else
	{
		if( !(*pte&PTE_P) )
	{
			print_back();
		return 0;
		}
	else
	{
			print_back();
		return 1;
		}
	}
}

void* get_a_page()
{
	unsigned int index;
	if( (index=bitmap_get_one(&kernel_page_pool.bitmap)) == BITMAP_FULL )
	{
		return NULL;
	}
	void* page=(kernel_page_pool.addr_start)+index*PAGE_SIZE;
	if( mapping_addr(page) != 0 )
	{
		return NULL;
	}
	return page;
}

static void* get_pages(unsigned int num)
{
	print_func();
	unsigned int index;
	unsigned int x;
	void* page=NULL;
	if( !((index=bitmap_gets(&kernel_page_pool.bitmap, num)) == BITMAP_FULL) )
	{
		for(x=0;x<num;x++)
	{
		if( !(check_addr_mapping(page)) )
		{
			if( mapping_addr((kernel_page_pool.addr_start)+(index+x)*PAGE_SIZE) == -1)
			{
					print_back();
				return NULL;
				}
			}
		}
	}
	else
	{
		print_back();
		return NULL;
	}
	print_back();
	return kernel_page_pool.addr_start+index*PAGE_SIZE;
}
		
void* get_mem(unsigned int size)
{
	size=(size+4-1)/4;
	static unsigned int bottom=PAGE_SIZE;
	static void* cur_page=NULL;
	if( size <= PAGE_SIZE )
	{
		if(bottom+size <= PAGE_SIZE)
		{
			bottom+=size;
		return cur_page+bottom-size;
		}
		else
		{
			cur_page=get_a_page();
		bottom=size;
		return cur_page;
		}
	}
	else
	{
		return get_pages((size+PAGE_SIZE-1)/PAGE_SIZE);
	}
}

