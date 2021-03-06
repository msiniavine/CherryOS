#ifndef MM_H
#define MM_H

#include <types.h>

#define PAGE_SIZE (4*1024)

#define PGD_SHIFT 22
#define PAGE_SHIFT 12
#define PTRS_PER_PGD 1024
#define PTRS_PER_PTE 1024

#define pgd_index(address) (((address) >> PGD_SHIFT) & (PTRS_PER_PGD - 1 ))
#define pgd_entry(pgd, address) ((pgd)+pgd_index(address))

#define pte_index(address) ((address) >> PAGE_SHIFT & (PTRS_PER_PTE - 1))
#define pfn(address) ((address) >> PAGE_SHIFT)
#define pfn_to_page(pfn) ((pfn) << PAGE_SHIFT)

// Page table entry for 32-bit paging with 4k pages
// as found in the Intel manual
struct pte
{
	u32     p:1,  // present
		rw:1,  // read/write
		us:1,  //user/supervisor
		a:1,   // accesses bit, cpu sets whis if the page is accessed
		d:1, // dirty, cpu sets this if the page was written to
		reserved:7, // must be set to 0
		addr:20; // physical address
} __attribute__((packed));

// Page directory entry
struct pde
{
	u32     p:1,   // present
		rw:1,  // read/write
		us:1,  // user/supervisor
		pwt:1,
		pcd:1, 
		a:1,
		reserved:6, // must be 0
		addr:20;  // address of the page table
} __attribute__((packed));

// Initializes memory management
// sets up identity paging for the first 8MB
void init_mm();
// Initializes the memory management data structures
void init_memory();

// Allocates and return the address of a free page
u32 get_free_page(void);
// Frees the page so it can be used again
void free_page(u32 page);
// Maps a page to a given virtual address
void put_page(u32 page, u32 address);

#endif
