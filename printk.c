#include <console.h>
#include <types.h>
#include <string.h>
#include <serial.h>

#define SIGN 1
#define PADDING 2
#define set_padding(flags, padding) (flags | PADDING | ((int)padding << PADDING))
#define get_padding(flags) ((char)(flags >> PADDING))

int itoak(char* buffer, size_t size, long n, int base, int flags)
{
	char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char tmp[66];
	int i = 0;
	size_t copied = 0;
	char* str = buffer;
	char sign = 0;

	if(flags & SIGN)
	{
		if(n < 0)
		{
			n=-n;
			sign = '-';
		}
		else
		{
			sign = 0;
		}
	}

	if(flags & PADDING)
	{
		memset(tmp, get_padding(flags), 8);
	}

	do
	{
		tmp[i] = digits[(unsigned long)n%(unsigned)base];
		n = (unsigned long)n/(unsigned) base;
		i++;
      	} while(n);

	if(sign)
	{
		tmp[i] = sign;
		i++;
	}

	if(flags & PADDING && i < 8)
	{
		i = 8;
	}

	while(i-- > 0)
	{
		*str++ = tmp[i];
		copied+=1;
		if(copied == size - 1)
			break;
	}

	buffer[copied] = '\0';
	return copied;
	
}

//  a version of print
// supports %s, %d, %u %p format specifiers
void printk(const char* format, ...)
{
	char buffer[1024];
	va_list args;

	int iformat, ibuffer;
	
	va_start(args, format);
	iformat = ibuffer = 0;

	while(format[iformat] != 0)
	{
		if(format[iformat] != '%')
		{
			buffer[ibuffer] = format[iformat];
			ibuffer+=1;
			iformat+=1;
		}
		else
		{
			char* str;
			// skip the percent sign
			iformat+=1;
			switch(format[iformat])
			{
			case 's':
				str = va_arg(args, char*);
				while(*str && ibuffer < 1024)
				{
					buffer[ibuffer] = *str;
					ibuffer+=1;
					str+=1;
				}
				iformat += 1;
				break;
			case 'd':
				ibuffer += itoak(&buffer[ibuffer], 1024-ibuffer, va_arg(args, int), 10, SIGN);
				iformat += 1;
				break;
			case 'u':
				ibuffer += itoak(&buffer[ibuffer], 1024-ibuffer, va_arg(args, unsigned int), 10, 0);
				iformat += 1;
				break;
			case 'x':
				ibuffer += itoak(&buffer[ibuffer], 1024-ibuffer, va_arg(args, unsigned int), 16, 0);
				iformat += 1;
				break;
			case 'p':
				ibuffer += itoak(&buffer[ibuffer], 1024-ibuffer, va_arg(args, unsigned int), 16, set_padding(0, '0'));
				iformat += 1;
				break;
			}
		}
	}
	buffer[ibuffer] = '\0';
	console_write(buffer, ibuffer);
	serial_write(buffer, ibuffer);
	va_end(args);
}

