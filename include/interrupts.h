#ifndef INTERRUPTS_H
#define INTERRUPTS_H

typedef void (*handler_func)();

// register a function to be called on an interrupt
void register_interrupt_handler(handler_func handler, int num);
#endif
