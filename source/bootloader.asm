bits 32
section .multiboot

magic    equ 0xE85250D6
arch     equ 0
length   equ 24
checksum equ -(magic + arch + length)

tagtypeend equ 0
tagsizeend equ 8

dd magic
dd arch
dd length
dd checksum
dd tagtypeend
dd tagsizeend

section .text.prologue
    global _start
    extern kmain
    extern pml4
    extern pdp
    extern pd
_start:
    cli
    mov esp, stackTop
    
    push 0
    push ebx

    mov edi, pml4
    mov eax, pdp
    or eax, 0x7    ; PRESENT, WRITABLE, USER
    mov [edi], eax

    mov edi, pdp
    mov eax, pd
    or eax, 0x7    ; PRESENT, WRITABLE, USER
    mov [edi], eax

    mov edi, pd
    mov eax, 0x83  ; PRESENT, WRITABLE, HUGE
    mov ecx, 512
.set_entry:
    mov [edi], eax
    add eax, 0x200000
    add edi, 8
    loop .set_entry

    mov eax, cr4
    or eax, 1 << 5  ; PAE bit
    mov cr4, eax

    mov eax, pml4
    mov cr3, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 0x100   ; LME bit
    wrmsr

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    lgdt [gdtr]

    jmp 0x08:gdt2

[BITS 64]

gdt2:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    pop rdi
    call kmain

    cli
hltLoop:
    hlt
    jmp hltLoop

gdtr:
    dw gdt_end - gdt_base - 1
    dq gdt_base
gdt_base:
    ; null segment
    dq 0
    ; 64-bit segment
    dw 0
    dw 0
    db 0
    db 0x9a
    db 0x20
    db 0
    ; 64-bit segment
    dw 0xffff
    dw 0
    db 0
    db 0x92
    db 0
    db 0
gdt_end:

section .bss
align 16
stackBottom:
    resb 16384
stackTop: