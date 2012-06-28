#include <string.h>

size_t strlen(const char* str)
{
	size_t len = 0;
	for(len = 0; str[len] != NULL; len++);
	return len;
}

void* memset(void* ptr, int value, size_t num)
{
	unsigned char* string = ptr;
	unsigned char val = value;
	size_t i;
	for(i = 0; i<num; i++)
	{
		string[i] = val;
	}

	return ptr;
}

void* memcpy(void* dest, void* src, size_t num)
{
	char* d = dest;
	char* s = src;
	size_t i;
	for(i = 0; i<num; i++)
	{
		d[i] = s[i];
	}

	return  dest;
}
