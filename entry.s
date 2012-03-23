
// Macro to generate an isr which does not automaticall
// push the error code, so it pushes 0 by itself
.macro isr_noerror name, code
.global \name
	\name:
	cli
	push $0
	push \code
	jmp isr_common
.endm

// macro to generate isr which pushes and error code
.macro isr_error name, code
.global \name
	\name:
	cli
	push \code
	jmp isr_common
.endm

isr_noerror isr0, $0
//isr_noerror isr1, $0
isr_noerror isr2, $2
isr_noerror isr3, $3
isr_noerror isr4, $4
isr_noerror isr5, $5
isr_noerror isr6, $6
isr_noerror isr7, $7
isr_error isr8, $8
//isr_noerror isr9, $0
isr_error isr10, $10
isr_error isr11, $11
isr_error isr12, $12
isr_error isr13, $13
isr_error isr14, $14
//isr_noerror isr15, $0
isr_noerror isr16, $16
isr_noerror isr17, $17
isr_noerror isr18, $18
isr_noerror isr19, $19


isr_common:
	pusha
	mov %ds, %eax
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

