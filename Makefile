CC=gcc
AS=as
LD=ld
INCLUDES=-Iinclude
CFLAGS=-O1 -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs $(INCLUDES)

OBJS=start.o kmain.o gdt.o string.o console.o printk.o entry.o interrupts.o mm_init.o memory.o timer.o serial.o

all: cherryos

cherryos: $(OBJS)
	$(LD) -T linker.ld -o cherryos $(OBJS)

clean:
	rm -f cherryos $(OBJS) *~ include/*~
