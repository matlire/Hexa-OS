[extern interrupt_handler]

global interrupt_stub

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

section .text

interrupt_stub:
	cli ; Reset interrupt IF flag

	; Save current CPU state to stack
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	pushfq ; Save FLAGS to stack

	mov rdi, rsp    ; Pass stack as argument for isr_handler
	call interrupt_handler ; Call handler

	; After interrupt handling, restore CPU state
	popfq
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax

	add rsp, 16 ; Clean stack: int_no and err_code (8+8=16) 

	sti   ; Reset IF flag
	iretq ; Return to program execution after interrupt

%macro ISR_NO_ERRCODE 1
isr%1:
    cli
    push 0 		 ; Error code
    push %1      ; Interrupt number
    jmp interrupt_stub
%endmacro

%macro ISR_ERRCODE 1
isr%1:
    cli
    push %1      ; Interrupt number (error code is already on stack from CPU)
    jmp interrupt_stub
%endmacro

%macro IRQ 1
irq%1:
    cli
    push byte %1      ; Raw IRQ number
    push byte %1 + 32 ; Vector
    jmp interrupt_stub
%endmacro

; ISR 0-7, 9, 15-31: No error code
ISR_NO_ERRCODE 0  ; 0: Divide By Zero Exception
ISR_NO_ERRCODE 1  ; 1: Debug Exception
ISR_NO_ERRCODE 2  ; 2: Non Maskable Interrupt Exception
ISR_NO_ERRCODE 3  ; 3: Int 3 Exception
ISR_NO_ERRCODE 4  ; 4: INTO Exception
ISR_NO_ERRCODE 5  ; 5: Out of Bounds Exception
ISR_NO_ERRCODE 6  ; 6: Invalid Opcode Exception
ISR_NO_ERRCODE 7  ; 7: Coprocessor Not Available Exception
ISR_NO_ERRCODE 9  ; 9: Coprocessor Segment Overrun Exception
ISR_NO_ERRCODE 15 ; 15: Reserved Exception
ISR_NO_ERRCODE 16 ; 16: Floating Point Exception
ISR_NO_ERRCODE 17 ; 17: Alignment Check Exception
ISR_NO_ERRCODE 18 ; 18: Machine Check Exception
ISR_NO_ERRCODE 19 ; 19: Reserved
ISR_NO_ERRCODE 20 ; 20: Reserved
ISR_NO_ERRCODE 21 ; 21: Reserved
ISR_NO_ERRCODE 22 ; 22: Reserved
ISR_NO_ERRCODE 23 ; 23: Reserved
ISR_NO_ERRCODE 24 ; 24: Reserved
ISR_NO_ERRCODE 25 ; 25: Reserved
ISR_NO_ERRCODE 26 ; 26: Reserved
ISR_NO_ERRCODE 27 ; 27: Reserved
ISR_NO_ERRCODE 28 ; 28: Reserved
ISR_NO_ERRCODE 29 ; 29: Reserved
ISR_NO_ERRCODE 30 ; 30: Reserved
ISR_NO_ERRCODE 31 ; 31: Reserved

; ISR 8, 10-14: Error code automatically pushed by CPU
ISR_ERRCODE 8  ; 8: Double Fault Exception (With Error Code!)
ISR_ERRCODE 10 ; 10: Bad TSS Exception (With Error Code!)
ISR_ERRCODE 11 ; 11: Segment Not Present Exception (With Error Code!)
ISR_ERRCODE 12 ; 12: Stack Fault Exception (With Error Code!)
ISR_ERRCODE 13 ; 13: General Protection Fault Exception (With Error Code!)
ISR_ERRCODE 14 ; 14: Page Fault Exception (With Error Code!)

IRQ 0  ; System timer
IRQ 1  ; PS/2 keyboard
IRQ 2  ; Cascade/Slave PIC
IRQ 3  ; COM2 / COM4
IRQ 4  ; COM1 / COM3
IRQ 5  ; Legacy Sound Card
IRQ 6  ; Floppy disk controller
IRQ 7  ; LPT1
IRQ 8  ; RTC
IRQ 9  ; IRQ2
IRQ 10 ; PCI
IRQ 11 ; PCI
IRQ 12 ; PS/2 mouse
IRQ 13 ; FPU
IRQ 14 ; Primary ATA Channel
IRQ 15 ; Secondary ATA Channel
