#include <string.h>
#include <kernel.h>
#include <mm.h>

void kmain(void* mdb, unsigned int magic)
{
	u32* a = (u32*)0xa0000000;
	printk("Welcome to CherryOS\n");

	// Set up 4MB of memory
	init_mm(0, 4*1024*1024);

	*a = 42;

	cpu_halt();
}

