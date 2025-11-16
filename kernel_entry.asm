; kernel_entry.asm
; Entry point for the C kernel - must be at address 0x1000
; The linker script handles address placement (ORG not needed for ELF)
BITS 16

global _start
extern main

_start:
    ; Clear segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000  ; Set up stack below bootloader region
    
    ; Call the C main function
    call main
    
    ; If main returns (shouldn't), hang
.hang:
    hlt
    jmp .hang

