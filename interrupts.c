#include <kernel.h>
#include <registers.h>

static void print_error(struct regs* regs)
{
	printk("\n");
	printk("FATAL ERROR!!!\n");
	printk("Unhandled Exception: %x Error Code %x\n", regs->int_no, regs->err);
	printk("EIP %p\n", regs->eip);
	printk("EFLAGS %p\n", regs->eflags);
	printk("eax: %x ebx: %x ecx: %x edx: %x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
	printk("esi: %x edi: %x ebp: %x esp: %x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
	printk("ds: %x cs: %x ss: %x\n", regs->ds, regs->cs, regs->ss);
	printk("user sp: %x\n", regs->useresp);
}

void isr_handler(struct regs regs)
{
	print_error(&regs);
	cpu_halt();
}
