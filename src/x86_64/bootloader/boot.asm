global _start
extern _long_mode_start

section .text   

bits 32

_start:
    mov esp, stack_top

    call check_multiboot
    call check_cpuid
    call check_long_mode

    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp gdt64.code_segment:_long_mode_start

    hlt

check_multiboot:
    cmp eax, 0x36d76289 ; Magicknumber
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "M"
    jmp error

; Here we try to flip 21'st bit of CPU EFLAGS register to check if cpu_id instructions supported
check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "C"
    jmp error

check_long_mode:
    mov eax, 0x80000000
    cpuid               ; Query highest extended CPUID function
    cmp eax, 0x80000001
    jl .no_long_mode
    mov eax, 0x80000001
    cpuid               ; Query extended feature bits
    test edx, 1 << 29   ; Check EDX[29] (LM bit = Long Mode support)
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, "L"
    jmp error

setup_page_tables:
    mov eax, page_table_l3
    or eax, 0b11
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or eax, 0b11
    mov [page_table_l3], eax

    mov eax, page_table_l1
    or  eax, 0b11
    mov [page_table_l2], eax

    mov ecx, 0
.loop:
    mov eax, ecx
    shl eax, 12                         ; eax = ecx * 0x1000 (4 KiB)
    or eax, 0b11                        ; Present + Writable
    mov [page_table_l1 + ecx * 8], eax  ; Map page by bytes
    inc ecx
    cmp ecx, 512                        ; Map 512 * 4 KiB = 2 MiB of memory 
    jne .loop

    ret

enable_paging:
    mov eax, page_table_l4
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

error:
    mov dword [0xb8000], 0x4f524f45 ; ER, white on red
    mov dword [0xb8004], 0x4f3a4f52 ; R:
    mov dword [0xb8008], 0x4f204f20 ;   
    mov byte [0xb800a], al          ; Input parameter, one letter
    hlt

section .bss
; Paging, 4 KiB per page
align 4096
page_table_l4: ; PML4
    resb 4096
page_table_l3: ; PDPT
    resb 4096
page_table_l2: ; PD
    resb 4096
page_table_l1: ; PT
    resb 4096

stack_bottom:
    resb 4096*4 ; 4 KiB * 4 = 16 KiB stack
stack_top:

section .rodata
gdt64:
    dq 0                                             ; Null descriptor
.code_segment: equ $ - gdt64                         ; offset = current address = 1dt64 + 8
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; Set parameters: granularity = 0; default operation size = 0 (ignored for lm); 64-bit code segment = 1; P = 1 - Execute/Read 
.pointer:
    dw $ - gdt64 - 1 ; Limit = size(GDT) − 1
    dq gdt64         ; Base address of GDT
