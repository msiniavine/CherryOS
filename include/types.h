#ifndef TYPES_H
#define TYPES_H

#define NULL 0

typedef int size_t;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;
typedef unsigned char u8;

typedef __builtin_va_list va_list;
#define va_start(a,b) __builtin_va_start((a),(b))
#define va_arg(a,b) __builtin_va_arg((a), b)
#define va_end(a) __builtin_va_end((a))

#endif
