#include <console.h>
#include <string.h>

// location in video memory
static u16* vram = (u16*)0xb8000;
// current write location
static int cx = 0;
static int cy = 0;

#define NCOLS 80
#define NROWS 25
#define CONSOLE_SIZE (NCOLS*NROWS)


// Scrolls the text up the screen so new lines could be added to the bottom
static void scroll()
{
	u8 space_attr = (0 << 4) | (15 & 0x0f);
	u16 space = 0x20 | (space_attr << 8);
	int row;
	int i;
	for(row = 1; row < NROWS; row++)
	{
		u16* src = &vram[row*NCOLS];
		u16* dest = &vram[(row-1)*NCOLS];

		memcpy(dest, src, sizeof(u16)*NCOLS);
		
	}

	// Fill the last line with spaces
	for(i = 24*NCOLS; i<NROWS*NCOLS;i++)
	{
		vram[i] = space;
	}

	// Set the position to the start of the last line
	cx = 0;
	cy = NROWS-1;
}

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
			scroll();
		}

		return;
	}

	if(c == '\n')
	{
		cx = 0;
		cy++;
		if(cy >= NROWS)
		{
			scroll();
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
