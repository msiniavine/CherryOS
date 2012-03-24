#include <string.h>
#include <kernel.h>
#include <mm.h>

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
	u64 addr;
	u64 len;
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
			printk(" size=%x, base_addr=%x%x,"
			       " length=%x%x, type=%x\n",
			       mmap->size, mmap->addr>>32, 
			       mmap->addr & 0xffffffff,
			       mmap->len >> 32,
			       mmap->len  & 0xffffffff,
			       mmap->type);
		}
	}
}

void kmain()
{
	printk("Welcome to CherryOS\n");
	print_mb_info(mbd);

	// Set up 4MB of memory
	init_mm(0, 4*1024*1024);

	cpu_halt();
}

