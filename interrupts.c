#include <kernel.h>
#include <string.h>
#include <registers.h>

// Interrupt descriptor entry
// As defined in the Intel manual
struct igate_desc
{
	u16 offset0;
	u16 selector;
	u8 reserved;
	u8 type: 5, dpl: 2, p: 1;
	u16 offset1;
	
} __attribute__((packed));


// Different types of interrupts
// Regular interrupts and traps
#define IGATE_INT_TYPE 0xe
#define IGATE_TRAP_TYPE 0xf

struct system_table_register idtr;
static struct igate_desc idt[256];


// Fills in the descriptor structure
// simialr to the fill_gdt_entry function
static void fill_idt_entry(struct igate_desc* entry, void* func, u16 selector, unsigned type)
{
	u32 offset = (u32)func;
	entry->offset0 = offset & 0xffff;
	entry->offset1 = (offset >> 16) & 0xffff;

	entry->type = type;
	entry->selector = selector; 

	entry->dpl = 0; // kernel privilige level
	entry->p = 1; // present
	entry->reserved = 0; // reserved is always 0
}

extern void divide_error(void);

// fill in the interrupt descriptor tables with the functions that will handle in interrupts
// the actual functions are defined in entry.s
// but all those functions just end up calling isr_handler below
void set_up_idt()
{
	memset(&idt[0], 0, sizeof(idt));
	idtr.address = (u32)&idt[0];
	idtr.limit = sizeof(idt)-1;

	fill_idt_entry(&idt[0], divide_error, 0x8, IGATE_TRAP_TYPE);
}


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
