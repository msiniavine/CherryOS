#include <types.h>
#include <string.h>

// GDTR structure
struct system_table_register
{
	u16 limit;
	u32 address;
} __attribute__((packed));


// entry in the descriptor table
struct desc_entry
{
	u16 limit0;
	u16 base0;

	u16 base1: 8, type: 4, s: 1, dpl: 2, p: 1;
	u16 limit1: 4, avl: 1, l: 1, db: 1, g: 1, base2: 8;
} __attribute__((packed));

// Constants for different descriptor types
// we only need two types, one for code, one for data
#define GDT_DATA_TYPE 2
#define GDT_CODE_TYPE 10


// entry into interrupt descriptor table
struct igate_desc
{
	u16 offset0;
	u16 selector;
	u8 reserved;
	u8 type: 5, dpl: 2, p: 1;
	u16 offset1;
	
} __attribute__((packed));

#define IGATE_INT_TYPE 0xe
#define IGATE_TRAP_TYPE 0xf

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

static void fill_gdt_entry(struct desc_entry* entry, u32 base, u32 limit, unsigned type)
{
	memset(entry, 0, sizeof(*entry));

	entry->limit0 = limit & 0xffff;
	entry->limit1 = (limit >> 16) & 0x0f;

	entry->base0 = base & 0xffff;
	entry->base1 = (base >> 16) & 0xff;
	entry->base2 = (base >> 24) & 0xff;

	entry->type = type;
	
	entry->s = 1;   // 1 for code or data segment
	entry->dpl = 0;  // for kernel privilige level
	entry->p = 1; // segment is present
	entry->db = 1; // this should always be 1 for 32-bit code
	entry->g = 1; // set maximum granularity to use the whole memory
	entry->l = 0; // not using 64-bit code
	entry->avl = 0; // reserver for os, so set to 0 since we are not using it
}

struct system_table_register gdtr;
struct system_table_register idtr;
static struct igate_desc idt[256];
static struct desc_entry gdt[3];

// fill in the gdtr and gdt tables
void set_up_gdt()
{
	memset(&gdt[0], 0, sizeof(gdt));

	// the entry at index 0 is zeroed out
	fill_gdt_entry(&gdt[1], 0, 0xffffffff, GDT_CODE_TYPE);
	fill_gdt_entry(&gdt[2], 0, 0xffffffff, GDT_DATA_TYPE);
	
	gdtr.address = (u32)&gdt[0];
	gdtr.limit = sizeof(gdt)-1;
	
}

// declare the assembly isr function
extern void asm_isr();

size_t printk(const char* str);
void isr()
{
	printk("!");
}

// fill in the interrupt descriptor tables
void set_up_idt()
{
	memset(&idt[0], 0, sizeof(idt));
	idtr.address = (u32)&idt[0];
	idtr.limit = sizeof(idt)-1;

	fill_idt_entry(&idt[0], asm_isr, 0x8, IGATE_TRAP_TYPE);
}

