#include <string.h>
#include <kernel.h>

void kmain(void* mdb, unsigned int magic)
{
	printk("hello world\n");
	printk("Hello world\n");

	printk("Number %d negative %d\n", 1337, -13565);
	printk("Unsigned %u %u\n", 42, -42);
	printk("Hex %x, %x pointer %p\n", magic, 0xdeadbeef, mdb);
	printk("String %s\n", "Hello world!");
	printk("Empty string: %s\n", "");
}

