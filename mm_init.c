#include <kernel.h>
#include <mm.h>
#include <string.h>

#define PAGE_SIZE (4*1024)

#define PGD_SHIFT 22
#define PAGE_SHIFT 12
#define PTRS_PER_PGD 1024
#define PTRS_PER_PTE 1024

#define pgd_index(address) (((address) >> PGD_SHIFT) & (PTRS_PER_PGD - 1 ))
#define pgd_entry(pgd, address) ((pgd)+pgd_index(address))

#define pte_index(address) ((address) >> PAGE_SHIFT & (PTRS_PER_PTE - 1))
#define pfn(address) ((address) >> PAGE_SHIFT)

static void set_up_ptes(struct pte* page_table, u32 start, u32 end)
{
	u32 address;
	for(address = start; address < end; address += PAGE_SIZE)
	{
		struct pte* pte = page_table+pte_index(address);
		pte->p = 1;
		pte->rw = 1;
		pte->us=0;
		pte->a = 0;
		pte->d = 0;
		pte->reserved = 0;
		pte->addr = pfn(address);
	}
}

static void set_pgd(struct pde* pgd)
{
	asm volatile ("mov %0, %%cr3" :: "r"(pgd));
}

static void enable_paging(void)
{
	u32 cr0;
	__asm__("mov %%cr0, %0\n"
		"or $0x80000000, %0\n"
		"mov %0, %%cr0\n" :: "r"(cr0));
}

// creates a pde using a give physical address pointing to a page table
static void make_pde(struct pde* pde, u32 address)
{
	pde->p=1;
	pde->rw=1;
	pde->us=0;
	pde->pwt = 0;
	pde->pcd=0;
	pde->a=0;
	pde->reserved=0;
	pde->addr = pfn(address);
}

struct pde pgd[1024] __attribute__((aligned(PAGE_SIZE)));
struct pte pg0[1024] __attribute__((aligned(PAGE_SIZE)));
struct pte pg1[1024] __attribute__((aligned(PAGE_SIZE)));

void init_mm()
{
	printk("Setting up identity mapping %p-%p\n", 0, 8*1024*1024);

	memset(pgd, 0, PAGE_SIZE);
	make_pde(pgd_entry(pgd, 0), (u32)pg0);
	make_pde(pgd_entry(pgd, 4*1024*1024), (u32)pg1);

	set_up_ptes(pg0, 0, 4*1024*1024);
	set_up_ptes(pg1, 4*1024*1024, 8*1024*1024);
	set_pgd(pgd);
	enable_paging();
}
