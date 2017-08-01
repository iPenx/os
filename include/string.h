#ifndef _STRING_H
#define _STRING_H

#include "stdint.h"

void memset(void*, uint8_t, uint32_t);
void memcpy(void*, const void*, uint32_t);
int memcmp(const void*, const void*, uint32_t);

unsigned int  strcpy(char*, const char*);
int strcmp(const char*, const char*);
char* strchar(const char*, const char);
char* strrchar(const char*, const char);
void strcat(char*, const char*);
uint32_t strchars(const char*, const char);
uint32_t strlen(const char* str);

#ifndef NULL
#define NULL (void*)0
#endif

#endif
