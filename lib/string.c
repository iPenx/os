#include "string.h"
#include "stdint.h"
#include "print.h"
#include "debug.h"

void memset(void* dst, uint8_t value, uint32_t size)
{
    print_func();
    while(size-->0)
        *(uint8_t*)dst++=value;
    print_back();
}

void memcpy(void* dst, const void* src, uint32_t size)
{
    ASSERT(dst!=NULL);
    ASSERT(src!=NULL);
    while(size-- > 0)
        *(uint8_t*)dst++=*(uint8_t*)src++;
}

int memcmp(const void* a, const void* b, uint32_t size)
{
    ASSERT(a!=NULL);
    ASSERT(b!=NULL);
    while(size-- >0)
        return (*(uint8_t*)a++ > *(uint8_t*)b++)? 1 : -1;
    return 0;
}

unsigned int strcpy(char* dst, const char* src)
{
    ASSERT(dst!=NULL);
    ASSERT(src!=NULL);
    unsigned int count;
    while(*dst++=*src++)
        count++;
    return count;
}

uint32_t strlen(const char* str)
{
    ASSERT(str!=NULL);
    const char* p=str;
    while(*p++);
    return (p-str-1);
}

int strcmp(const char* a, const char* b)
{
    ASSERT(a!=NULL);
    ASSERT(b!=NULL);
    while(*a || *b)
        return (*a++ > *b++)? 1 : -1;
    return 0;
}

char* strchar(const char* str, const char ch)
{
    ASSERT(str!=NULL);
    while(*str!=0)
    {
        if(*str==ch)
	    return (char*)str;
        str++;
    }
    return NULL;
}

char* strrchar(const char* str, const char ch)
{
    ASSERT(str!=NULL);
    char* last_char=NULL;
    while(*str!=0)
    {
        if(*str==ch)
	    last_char=(char*)str;
        str++;
    }
    return last_char;
}

void strcat(char* dst, const char* src)
{
    ASSERT(dst!=NULL);
    ASSERT(src!=NULL);
    while(*dst++);
    --dst;
    while(*dst++=*src++);
}

uint32_t strchars(const char* str, const char ch)
{
    ASSERT(str!=NULL);
    uint32_t count_ch=0;
    while(*str)
    {
        if(*str==ch)
	    count_ch++;
        str++;
    }
    return count_ch;
}
