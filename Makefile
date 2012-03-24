CC=gcc
AS=as
LD=ld
INCLUDES=-Iinclude
CFLAGS=-c -O1 -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs $(INCLUDES)

all: cherryos

cherryos: start.o kmain.o gdt.o string.o console.o printk.o entry.o interrupts.o mm.o
	$(LD) -T linker.ld -o cherryos start.o kmain.o gdt.o string.o console.o printk.o entry.o interrupts.o mm.o

start.o: start.s
	$(AS) -o start.o start.s

entry.o: entry.s
	$(AS) -o entry.o entry.s

kmain.o: kmain.c
	$(CC) $(CFLAGS) -o kmain.o kmain.c

gdt.o: gdt.c
	$(CC) $(CFLAGS) -o gdt.o gdt.c

string.o: string.c
	$(CC) $(CFLAGS) -o string.o string.c

console.o: console.c
	$(CC) $(CFLAGS) -o console.o console.c

printk.o: printk.c
	$(CC) $(CFLAGS) -o printk.o printk.c

mm.o: mm_init.c
	$(CC) $(CFLAGS) -o mm.o mm_init.c

clean:
	rm -f cherryos kmain.o start.o gdt.o string.o console.o printk.o entry.o interrupts.o mm.o *~ include/*~
