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

// Interrupt segment selector
#define ISEL 0x8

struct system_table_register idtr;
static struct igate_desc idt[256];


// Fills in the descriptor structure
// similar to the fill_gdt_entry function
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

// declare assembly interrupt handling functions
extern void isr0(void);
//extern void isr1(void);  RESERVED
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
// extern void isr9(void); This is exception is no longer used
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
//extern void isr15(void);  RESERVED
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);

// irq declarations
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);


static void init_pic()
{
	// Remap the irq table.
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

// fill in the interrupt descriptor tables with the functions that will handle in interrupts
// the actual functions are defined in entry.s
// but all those functions just end up calling isr_handler below
void set_up_idt()
{
	memset(&idt[0], 0, sizeof(idt));
	idtr.address = (u32)&idt[0];
	idtr.limit = sizeof(idt)-1;

	fill_idt_entry(&idt[0], isr0, ISEL, IGATE_TRAP_TYPE);
//	fill_idt_entry(&idt[1], isr1, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[2], isr2, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[3], isr3, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[4], isr4, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[5], isr5, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[6], isr6, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[7], isr7, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[8], isr8, ISEL, IGATE_TRAP_TYPE);
//	fill_idt_entry(&idt[9], isr9, ISEL, IGATE_TRAP_TYPE);

	fill_idt_entry(&idt[10], isr10, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[11], isr11, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[12], isr12, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[13], isr13, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[14], isr14, ISEL, IGATE_TRAP_TYPE);
//	fill_idt_entry(&idt[15], isr15, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[16], isr16, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[17], isr17, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[18], isr18, ISEL, IGATE_TRAP_TYPE);
	fill_idt_entry(&idt[19], isr19, ISEL, IGATE_TRAP_TYPE);

	fill_idt_entry(&idt[32], irq0, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[33], irq1, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[34], irq2, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[35], irq3, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[36], irq4, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[37], irq5, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[38], irq6, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[39], irq7, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[40], irq8, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[41], irq9, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[42], irq10, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[43], irq11, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[44], irq12, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[45], irq13, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[46], irq14, ISEL, IGATE_INT_TYPE);
	fill_idt_entry(&idt[47], irq15, ISEL, IGATE_INT_TYPE);

	init_pic();
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

int tick = 0;
void irq_handler(struct regs regs)
{
	if(regs.int_no >= 40)
	{
		outb(0xa0, 0x20);
	}
	outb(0x20, 0x20);

	if(tick % 100 == 0)
		printk("Tick: %d\n", tick/100);
	tick++;
}

void init_timer()
{
	u32 divisor = 1193180/100;
	u8 l = divisor & 0xff;
	u8 h = (divisor >> 8) & 0xff;
	outb(0x43, 0x36);
	outb(0x40, l);
	outb(0x40, h);
}


