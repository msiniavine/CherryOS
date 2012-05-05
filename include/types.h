#ifndef TYPES_H
#define TYPES_H

#define NULL 0

// Define types of fixed lengths
typedef unsigned long size_t;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;
typedef unsigned char u8;

// Definitions for functions to handle variable number of arguments
typedef __builtin_va_list va_list;
#define va_start(a,b) __builtin_va_start((a),(b))
#define va_arg(a,b) __builtin_va_arg((a), b)
#define va_end(a) __builtin_va_end((a))

// GDTR and IDTG structure
// Used to set up the location of segment table and interrupt table
struct system_table_register
{
	u16 limit;
	u32 address;
} __attribute__((packed));


#endif
