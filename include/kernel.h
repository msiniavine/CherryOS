#ifndef KERNEL_H
#define KERNEL_H

void printk(const char* format, ...);

// Disables interrupts and stops the cpu
extern void cpu_halt(void);

#endif
