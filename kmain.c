#include <string.h>
#include <kernel.h>
#include <mm.h>

void kmain(void* mdb, unsigned int magic)
{
	int i = 0;
	printk("Welcome to CherryOS\n");

	// Set up 4MB of memory
	init_mm(0, 4*1024*1024);

	cpu_halt();
}

