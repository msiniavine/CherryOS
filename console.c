#include <console.h>

// location in video memory
static u16* vram = (u16*)0xb8000;
// current write location
static int cx = 0;
static int cy = 0;

#define NCOLS 80
#define NROWS 25
#define CONSOLE_SIZE (NCOLS*NROWS)

static void putchar(char c)
{
	if(c >= 0x20) // 0x20 is space
	{
		u16 glyph = (0x07 << 8) | c;
		vram[cy*NCOLS + cx] = glyph;
		cx++;
		if(cx >= NCOLS)
		{
			cx = 0;
			cy++;
		}

		if(cy >= NROWS)
		{
			cy = 0;
		}

		return;
	}

	if(c == '\n')
	{
		cx = 0;
		cy++;
		if(cy >= NROWS)
		{
			cy = 0;
		}
	}
}

void console_write(const char* str, size_t len)
{
	size_t i;	
	for(i=0; i<len; i++)
	{
		putchar(str[i]);
	}
}
