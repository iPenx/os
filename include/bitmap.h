#ifndef _LIB_BITMAP_H
#define _LIB_BITMAP_H

#include "stdint.h"

#define BITMAP_SUCCESS 0
#define BITMAP_FULL 0xFFFFFFFF
#define BITMAP_NOTFOUND 0xFFFFFFFF

#define BITMAP_ERROR_EXCEED 0xFFFFFFFF
#define BITMAP_ERROR_NOMEN 0x1

struct bitmap
{
    uint8_t* bit;
    uint32_t num;
};

extern int bitmap_init(struct bitmap* bitmap, unsigned int num);
extern unsigned int bitmap_lookup_one(struct bitmap* bitmap);
extern uint32_t bitmap_get_one(struct bitmap* bitmap);
extern uint32_t bitmap_gets(struct bitmap* bitmap, unsigned int num);
int bitmap_query(struct bitmap* bitmap, unsigned int index);
int bitmap_mask(struct bitmap* bitmap, unsigned int index);
int bitmap_umask(struct bitmap* bitmap, unsigned int index);

#endif
