[BITS 16]
[ORG 0x7C00]

jmp short start
nop
times 33 db 0

start:
    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00

    call load_kernel
    call enter_pm

load_kernel:
    mov si, disk_packet
    mov word [si+4], 128
    mov dl, [BOOT_DRIVE]
    int 0x13
    ret

disk_packet:
    db 0x10,0
    dw 0
    dw 0x1000
    dq 2

BOOT_DRIVE db 0

enter_pm:
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:flush

[BITS 32]
flush:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov esp, 0x9FC00
    call p_kstart
.hang:
    hlt
    jmp .hang

gdt_start:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start
gdt_end:

times 510-($-$$) db 0
dw 0xAA55
