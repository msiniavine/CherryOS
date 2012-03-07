CC=gcc
AS=as
LD=ld

all: cherryos

cherryos: start.o kmain.o linker.ld
	$(LD) -T linker.ld -o cherryos start.o kmain.o

start.o: start.s
	$(AS) -o start.o start.s

kmain.o: kmain.c
	$(CC) -c -o kmain.o kmain.c -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs

clean:
	rm -f cherryos kmain.o start.o *~
