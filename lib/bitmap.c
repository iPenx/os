#include "stdint.h"
#include "bitmap.h"
#include "memory.h"
#include "debug.h"

#define BITMAP_MASK 1

unsigned int  bitmap_lookup_one(struct bitmap* bitmap)
{
    unsigned int index_byte, index_bit;
    uint8_t* bit=bitmap->bit;
    unsigned int max_num=bitmap->num;
    for(index_byte=0;((index_byte<<3) < max_num) && (bit[index_byte] == 0xff);index_byte++)
    {
        ;
    }
    if((index_byte<<3) >= max_num)
    {
	    return BITMAP_ERROR;
    }
    for(index_bit=0;;index_bit++)
    {
        if( !(bit[index_byte]&(BITMAP_MASK<<index_bit)) )
	{
	    return index_byte*8+index_bit;
        }
    }
}

unsigned int bitmap_get_one(struct bitmap* bitmap)
{
    print_func();
    unsigned int index_byte, index_bit;
    uint8_t* bit=bitmap->bit;
    unsigned int max_num=bitmap->num;
    for(index_byte=0;((index_byte<<3) < max_num) && (bit[index_byte] == 0xff);index_byte++)
    {
        ;
    }
    if( (index_byte<<3) >= max_num )
    {
	print_back();
        return BITMAP_ERROR;
    }
    for(index_bit=0;;index_bit++)
    {
        if( !(bit[index_byte]&(BITMAP_MASK<<index_bit)) )
	{
	    bit[index_byte]|=(BITMAP_MASK<<index_bit);
	    print_back();
	    return index_byte*8+index_bit;
        }
    }
}

unsigned int bitmap_gets(struct bitmap* bitmap, unsigned int num)
{
    unsigned int index_byte, index_bit;
    unsigned int tmp_byte, tmp_bit;
    uint8_t* bit=bitmap->bit;
    unsigned int max_num=bitmap->num;
    unsigned int tmp_num=0;
    for(index_byte=0;(index_byte<<3) < max_num;index_byte++)
    {
	if(bit[index_byte] == 0xff)
	{
	    tmp_num=0;
	    continue;
        }
	tmp_byte=index_byte;
        for(index_bit=0;;index_bit++)
	{
	    if( bit[index_byte]&(BITMAP_MASK<<index_bit) )
	    {
	        tmp_num=0;
            }
	    else
	    {
		if(tmp_num==0)
		{
		    tmp_byte=index_byte;
		    tmp_bit=tmp_bit;
                }
                tmp_num++;
		if(tmp_num == num)
		{
                    return tmp_byte*8+tmp_bit;
		}
            }
        }
    }
    return BITMAP_ERROR;
}

int bitmap_query(struct bitmap* bitmap, unsigned int num)
{
    if(num >= bitmap->num)
        return -1;
    unsigned int index_byte=num/8;
    unsigned int index_bit=num%8;
    return (bitmap->bit[index_byte]&(BITMAP_MASK<<index_bit))>>index_bit;
}

int bitmap_set(struct bitmap* bitmap, unsigned int num)
{
    if(num >= bitmap->num)
        return -1;
    int index_byte=num/8;
    int index_bit=num%8;
    uint8_t* bit=(bitmap->bit)+index_byte;
    *bit|=(BITMAP_MASK<<index_bit);
    return 1;
}

int bitmap_clear(struct bitmap* bitmap, unsigned int num)
{
    if(num >= bitmap->num)
        return -1;
    unsigned int index_byte=num/8;
    unsigned int index_bit=num%8;
    uint8_t* bit=(bitmap->bit)+index_byte;
    *bit&=~(BITMAP_MASK<<index_bit);
    return 1;
}

int bitmap_init(struct bitmap* bitmap, unsigned int num)
{
    if( !(num < BITMAP_MAX_NUM) )
    {
        return -1;
    }
    if((bitmap->bit=get_mem((num+8-1)>>3)) == NULL) 
    {
        return -1;
    }
    bitmap->num=num;
    uint8_t* last_byte=(bitmap->bit)+((num-1)/8);
    int count_set=(8-num-((num>>3)<<3))%8;
    unsigned int index=7;
    for(;count_set--;index--)
    {
        *last_byte|=BITMAP_MAX_NUM<<index;
    }
    return 1;
}

