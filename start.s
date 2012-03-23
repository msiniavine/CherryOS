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

	call set_up_gdt

	lgdt gdtr
	mov $0x10, %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs
	mov %eax, %ss
	ljmp $(0x8), $cs_set
cs_set:
	call set_up_idt
	lidt idtr
	
	push %eax
	push %ebp

	call kmain
.global cpu_halt
cpu_halt:	
	cli
	hlt
	jmp cpu_halt
