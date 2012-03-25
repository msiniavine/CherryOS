#include <mm.h>
#include <string.h>

#define MEM_START 0
#define MEM_END  (8*1024*1024)
#define MEM_SIZE (MEM_END-MEM_START)
#define MEM_PAGES (MEM_SIZE/PAGE_SIZE)

// Start of the heap
#define HEAP_START (2*1024*1024)

#define bindex(pfn) (((pfn) >> 5) & 0x3f)
#define bmask(pfn)  (1 << ((pfn) & 0x1f))


// Bitmap to track memory usage
static int bitmap[64];

// Sets a bit in the bitmap to indicate that the page is being used for something
static void mark_page_used(u32 pfn)
{
	bitmap[bindex(pfn)] |= bmask(pfn);	
}

// checks the bitmap to see if a give page is free or not
static int in_use(u32 pfn)
{
	return bitmap[bindex(pfn)] & bmask(pfn);
}


// when allocating the page fill it with 0xa5 to help catch bugs
static void debug_allocate(u32 address)
{
	memset((void*)address, 0xa5, PAGE_SIZE);
}

// When freeing fill it with 0xb6
static void debug_free(u32 address)
{
	memset((void*)address, 0xb6, PAGE_SIZE);
}

// Mark the first 2 mb as occupied by the kernel
// leave the rest free
void init_memory()
{
	int i;
	memset(bitmap, 0, sizeof(bitmap));
	for(i = pfn(MEM_START); i < pfn(HEAP_START); i++)
	{
		mark_page_used(i);
	}
}

// scan the bitmap to locate a free page
u32 get_free_page()
{
	int i;
	for(i=pfn(HEAP_START); i<pfn(MEM_END);i++)
	{
		if(!in_use(i))
		{
			mark_page_used(i);
			debug_allocate(pfn_to_page(i));
			return pfn_to_page(i);
		}
	}

	return 0;
}


void free_page(u32 address)
{
	debug_free(address);
	bitmap[bindex(pfn(address))] &= ~(bmask(pfn(address)));
}
