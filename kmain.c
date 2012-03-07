#define NULL 0

typedef int size_t;
typedef unsigned short u16;

size_t strlen(const char* str)
{
	size_t len = 0;
	for(len = 0; str[len] != NULL; len++);
	return len;
}

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
	printk("hello world");
}

