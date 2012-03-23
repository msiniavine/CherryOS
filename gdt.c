#include <types.h>
#include <string.h>
#include <console.h>

// Set up the segment descriptor table
// Try to avoid using the segmentation as much as possible and
// instead rely on paging for memory management
// So the code here only sets up the segments for the flat 
// memory model.

// Entry in the descriptor table
// Corresponds to the spec in the Intel manual
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


// Assigns values to all the different fields in the segment descriptor
// Lots of shifts and bit masks are needed because of the strange layout 
// of the descriptor structure
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

// This is the actual descriptor table
// The first entry are all zeroes
// and the rest are the code and data segment descriptors
static struct desc_entry gdt[3]; 

// Fill in the segment descriptors for the code and data
// and let the cpu know where to find the descriptor table
void set_up_gdt()
{
	memset(&gdt[0], 0, sizeof(gdt));

	// the entry at index 0 is zeroed out
	fill_gdt_entry(&gdt[1], 0, 0xffffffff, GDT_CODE_TYPE);
	fill_gdt_entry(&gdt[2], 0, 0xffffffff, GDT_DATA_TYPE);
	
	gdtr.address = (u32)&gdt[0];
	gdtr.limit = sizeof(gdt)-1;
	
}


