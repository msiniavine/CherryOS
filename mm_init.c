#include <kernel.h>
#include <mm.h>
#include <string.h>

#define PAGE_SIZE (4*1024)

void init_mm(u32 start, u32 end)
{
	struct pde* pgd;
	int num_ptes;
	int num_page_tables;
	printk("Setting up identity mapping %x-%x\n", start, end);

	pgd = (struct pde*)(end - PAGE_SIZE);
	memset(pgd, 0, PAGE_SIZE);

	printk("PGD is at %p size pde %u size pte %u\n", pgd, sizeof(struct pde), sizeof(struct pte));

	num_ptes = (end-start)/PAGE_SIZE;
	num_page_tables = num_ptes * sizeof(struct pte) / PAGE_SIZE;
	printk("Need space for %d page tables\n", num_page_tables);
	
	
}
