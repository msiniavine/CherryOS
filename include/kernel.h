#ifndef KERNEL_H
#define KERNEL_H

#include <types.h>

void printk(const char* format, ...);

// Disables interrupts and stops the cpu
extern void cpu_halt(void);

static inline void outb(u16 port, u8 value)
{
	asm volatile ("outb %1, %0" :: "dN"(port), "a"(value));
}

static inline u8 inb(u16 port)
{
	u8 ret;
	asm volatile ("inb %1, %0" :"=a"(ret) : "dN"(port));
	return ret;
}

static inline u16 inw(u16 port)
{
	u16 ret;
	asm volatile ("inw %1, %0":"=a"(ret): "dN"(port));
	return ret;
}

static inline void sti()
{
	asm volatile ("sti");
}

static inline void cpu_relax()
{
	asm volatile ("hlt");
}

#endif
