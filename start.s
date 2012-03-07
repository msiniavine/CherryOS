.section .text
.global start
	.align 4
	.set ALIGN,	1
	.set MEMINFO,	2
	.set FLAGS,	ALIGN|MEMINFO
	.set MAGIC,	0x1badb002
	.set CHECKSUM,	-(MAGIC + FLAGS)

	.long MAGIC
	.long FLAGS
	.long CHECKSUM


	.set STACKSIZE,	0x4000
	.comm stack, STACKSIZE, 32

start:

	mov $(stack + STACKSIZE), %esp
	push %eax
	push %ebp

	call kmain

	cli

hang:
	hlt
	jmp hang

	