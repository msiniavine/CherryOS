#include <timer.h>
#include <interrupts.h>
#include <kernel.h>


int tick = 0;
void timer_interrupt()
{
	if(tick % HZ == 0)
		printk("Tick: %d\n", tick/HZ);
	tick++;

}

void init_timer()
{
	u32 divisor = 1193180/HZ;
	u8 l = divisor & 0xff;
	u8 h = (divisor >> 8) & 0xff;
	outb(0x43, 0x36);
	outb(0x40, l);
	outb(0x40, h);

	register_interrupt_handler(timer_interrupt, 32);
}

