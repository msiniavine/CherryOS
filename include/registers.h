#ifndef REGISTERS_H
#define REGISTERS_H

#include <types.h>

// Structure that defines the layout of registers as they are pushed on the stack by an interrupt handler
struct regs
{
	u32 ds;
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 esp;
	u32 ebx;
	u32 edx;
	u32 ecx;
	u32 eax;
	u32 int_no;
	u32 err;
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 useresp;
	u32 ss;
};

#endif
