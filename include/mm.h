#ifndef MM_H
#define MM_H

#include <types.h>

// Page table entry for 32-bit paging with 4k pages
// as found in the Intel manual
struct pte
{
	u32     p:1,  // present
		rw:1,  // read/write
		us:1,  //user/supervisor
		pwt:1, // page level write through
		pcd:1, // page cache disabled
		a:1,   // accesses bit, cpu sets whis if the page is accessed
		d:1, // dirty, cpu sets this if the page was written to
		reserved:7, // must be set to 0
		addr:20; // physical address
};

// Page directory entry
struct pde
{
	u32     p:1,   // present
		rw:1,  // read/write
		us:1,  // user/supervisor
		pwt:1,
		pcd:1, 
		a:1,
		reserved:8, // must be 0
		addr:20;  // address of the page table
};

#endif
