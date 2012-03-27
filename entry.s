
// Macro to generate an isr which does not automatically
// push the error code, so it pushes 0 by itself
.macro isr_noerror name, code
.global \name
	\name:
	cli
	push $0
	push \code
	jmp isr_common
.endm

// macro to generate isr which pushes an error code
.macro isr_error name, code
.global \name
	\name:
	cli
	push \code
	jmp isr_common
.endm

// macro to generate an irq handler
.macro irq name, gate
.global \name
	\name:
	cli
	push $0
	push \gate
	jmp irq_common
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

irq irq0, $32
irq irq1, $33
irq irq2, $34
irq irq3, $35
irq irq4, $36
irq irq5, $37
irq irq6, $38
irq irq7, $39
irq irq8, $40
irq irq9, $41
irq irq10, $42
irq irq11, $43
irq irq12, $44
irq irq13, $45
irq irq14, $46
irq irq15, $47


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

irq_common:
	pusha
	mov %ds, %eax
	push %eax
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call irq_handler

	pop %ebx
	mov %bx, %ds
	mov %bx, %es
	mov %bx, %fs
	mov %bx, %gs

	popa
	add $8, %esp
	sti
	iret

	