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

void init_mm(u32 start, u32 end)
{
	struct pde* pgd;
	struct pde* pde;
	struct pte* page_table;
	int num_ptes;
	int num_page_tables;
	printk("Setting up identity mapping %x-%x\n", start, end);

	pgd = (struct pde*)(end - PAGE_SIZE);
	memset(pgd, 0, PAGE_SIZE);

	num_ptes = (end-start)/PAGE_SIZE;
	num_page_tables = num_ptes * sizeof(struct pte) / PAGE_SIZE;

	page_table = (struct pte*)(end - 2*PAGE_SIZE);
	memset(page_table, 0, PAGE_SIZE);
	pde = pgd_entry(pgd, (u32)page_table);

	pde->p=1;
	pde->rw=1;
	pde->pwt=1;
	pde->addr = pfn((u32)page_table);

	set_up_ptes(page_table, start, end);
	set_pgd(pgd);
	enable_paging();
}
