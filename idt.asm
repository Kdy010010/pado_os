[GLOBAL p_idt_init]
[GLOBAL p_register_interrupt_handler]
[GLOBAL p_pic_unmask]
[EXTERN p_gdt_flush]

%define IDT_ENT 256

SECTION .bss
align 4
p_idt_table: resq IDT_ENT

SECTION .data
p_handlers: times IDT_ENT dd 0

SECTION .text
[BITS 32]

p_pic_unmask:
    mov dx,0x21
    in  al,dx
    mov cl,[esp+4]
    btr ax,cl
    out dx,al
    ret

isr_common:
    pusha
    push ds
    push esp
    mov ax,0x10
    mov ds,ax
    push esp
    call isr_dispatch
    add esp,4
    pop ds
    popa
    add esp,4
    iret

isr_noerr:
    push 0
    jmp isr_common

%macro ISR_STUB 1
global isr%1
isr%1:
    push %1
    jmp isr_noerr
%endmacro

%assign i 0
%rep 32
    ISR_STUB i
%assign i i+1
%endrep

global isr32
isr32:
    push 32
    jmp isr_noerr

global isr90
isr90:
    push 90
    jmp isr_noerr

isr_dispatch:
    pushad
    mov eax,[esp+36]
    mov ebx,[p_handlers+eax*4]
    test ebx,ebx
    jz .done
    push esp
    call ebx
    add esp,4
.done:
    popad
    ret

p_register_interrupt_handler:
    mov [p_handlers+eax*4],edx
    ret

p_idt_init:
    call p_gdt_flush
    lidt [p_idt_desc]
    ret

SECTION .data
p_idt_desc:
    dw IDT_ENT*8-1
    dd p_idt_table
