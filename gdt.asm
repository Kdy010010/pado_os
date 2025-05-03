[GLOBAL p_gdt_flush]
[EXTERN p_gdt_ptr]

SECTION .data
p_gdt:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
p_gdt_end:

p_gdt_ptr:
    dw p_gdt_end - p_gdt - 1
    dd p_gdt

SECTION .text
[BITS 32]
p_gdt_flush:
    lgdt [p_gdt_ptr]
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    jmp 0x08:.flush_ok
.flush_ok:
    ret
