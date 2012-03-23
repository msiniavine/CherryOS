.macro isr_noerror name, code
.global \name
	\name:
	cli
	push $0
	push \code
	jmp isr_common
.endm

.macro isr_error name, code
.global \name
	\name
	cli
	pushb \code
	jmp isr_common
.endm

isr_noerror divide_error, $0

isr_common:
	pusha
	mov %ds, %ax
	push %eax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call isr_handler

	pop %eax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	popa
	add $8, %esp
	sti
	iret

