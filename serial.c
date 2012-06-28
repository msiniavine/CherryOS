#include <kernel.h>
#include <serial.h>
#include <interrupts.h>


#define PORT 0x3f8   // COM1

static int is_init = 0;

static void serial_interrupt()
{
	printk("Serial interrupt\n");
}

void init_serial()
{

	outb(PORT+1, 0); // disable interrupts


	outb(PORT+3, 0x80); // enable DLAB, so baud rate could be set
	outb(PORT+0, 0x01); // Set devisor to 1 (low bit)
	outb(PORT+1, 0x00); // (high bit)
	outb(PORT+3, 0x03); // 8 bit no parity one stop bit
	outb(PORT+2, 0xc7); // enable fifo
	outb(PORT+4, 0x0b); // enable interrupts

	register_interrupt_handler(serial_interrupt, 35);
	register_interrupt_handler(serial_interrupt, 36);

	is_init = 1;
}

static int is_transmit_empty()
{
	return inb(PORT+5) & 0x20;
}

void serial_write(const void* buff, size_t num)
{
	const char* data = buff;
	size_t i;

	if(!is_init) return;

	for(i=0; i<num; i++)
	{
		while(is_transmit_empty() == 0);
		outb(PORT, data[i]);
	}
}



