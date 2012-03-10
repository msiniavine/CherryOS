#include <types.h>
#include <string.h>


size_t printk(const char* str)
{
	u16* videoram = (u16*)0xb8000;
	size_t len = strlen(str);
	size_t i;
	
	for(i=0; i<len; i++)
	{
		u16 glyph = (0x07 << 8) | str[i];
		videoram[i] = glyph;
	}

	return len;
	
}

void kmain(void* mdb, unsigned int magic)
{
	int i = 42;
	printk("hello world");

	i = i/0;
}

