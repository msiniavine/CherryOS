#ifndef STRING_H
#define STRING_H

#include <types.h>

size_t strlen(const char* str);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, void* src, size_t num);

#endif
