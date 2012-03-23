#include <kernel.h>
#include <registers.h>

void isr_handler(struct regs regs)
{
	printk("Inside interrupt %x error %x\n", regs.int_no, regs.err);
}
