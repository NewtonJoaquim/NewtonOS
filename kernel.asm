; kernel.asm
; Built as a flat binary placed immediately after the boot sector.
BITS 16
ORG 0x1000        ; Must match the load address ES:BX in the bootloader

start:
    mov si, msg
    mov ah, 0x0E
.print:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .print
.done:
    hlt
    jmp .done

msg db "Hello from kernel!", 0