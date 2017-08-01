#ifndef _LIB_BITMAP_H
#define _LIB_BITMAP_H

#include "stdint.h"
#include "string.h"

#define BITMAP_MAX_NUM 0xFFFFFFFF
#define BITMAP_ERROR 0xFFFFFFFF
struct bitmap
{
    uint8_t* bit;
    unsigned int num;
};

unsigned int bitmap_lookup_one(struct bitmap* bitmap);
unsigned int bitmap_get_one(struct bitmap* bitmap);
unsigned int bitmap_gets(struct bitmap* bitmap, unsigned int num);
int bitmap_query(struct bitmap* bitmap, unsigned int index);
int bitmap_set(struct bitmap* bitmap, unsigned int index);
int bitmap_clear(struct bitmap* bitmap, unsigned int index);
int bitmap_init(struct bitmap* bitmap, unsigned int num);

#endif
