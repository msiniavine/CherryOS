#include <string.h>
#include <kernel.h>
#include <mm.h>
#include <timer.h>
#include <serial.h>

// Multiboot information structure
struct mb_info
{
	u32 flags;
	u32 mem_lower;
	u32 mem_upper;
	u32 boot_device;
	char* cmdline;
	u32 mods_count;
	u32 mods_addr;
	u32 syms[4];
	
	u32 mmap_length;
	u32 mmap_addr;
};

struct mb_mmap_entry
{
	u32 size;
	u32 addr_low, addr_high;
	u32 len_low, len_high;
	u32 type;
} __attribute__((packed));

struct mb_info* mbd;

static void print_mb_info(struct mb_info* info)
{
	u32 flags = info->flags;
	printk("Flags %x\n", flags);
	if(flags & (1<<0))
		printk("mem_lowerp %uk mem_upper %uk\n", info->mem_lower, info->mem_upper);

	if(flags & (1<<6))
	{
		struct mb_mmap_entry* mmap;
		printk("mmap_length %u, mmap_addr %p\n", info->mmap_length, info->mmap_addr);
		for(mmap = (struct mb_mmap_entry*)info->mmap_addr;
		    (u32)mmap < info->mmap_addr+info->mmap_length;
		    mmap = (struct mb_mmap_entry*)((u32)mmap+mmap->size + sizeof(mmap->size)))
		{
			printk(" size=%x, base_addr=%p%p,"
			       " length=%p%p, type=%x\n",
			       mmap->size,
			       mmap->addr_high, mmap->addr_low,
			       mmap->len_high, mmap->len_low,
			       mmap->type
				);
		}
	}
}

void panic(const char* msg)
{
	printk("panic - %s", msg);
	cpu_halt();
}

void kmain()
{
	u32* pages;
	int i;
	printk("Welcome to CherryOS\n");
	print_mb_info(mbd);

	// Set up 4MB of memory
	init_mm();

	pages = (u32*)get_free_page();
	if(!pages)
	{
		panic("Could not allocate test page\n");
	}
	printk("Pages %p\n", pages);
	for(i=0; i<1024; i++)
	{
		pages[i] = get_free_page();
		if(!pages[i])
			panic("Failed to test\n");
	}
	printk("Now attempt to free\n");
	for(i=0; i<1024; i++)
	{
		free_page(pages[i]);
	}
	free_page((u32)pages);

	init_timer();
	init_serial();

	sti();

	serial_write("hello world serial", strlen("hello world serial"));
	for(;;) cpu_relax();

	cpu_halt();
}

