; kernel_entry.asm
; Entry point for the C kernel - must be at address 0x1000
; The linker script handles address placement (ORG not needed for ELF)
BITS 32

global _start
extern main

_start:
    ; We're already in protected mode with segments set up by bootloader
    ; Set up the stack
    mov esp, 0x9000
    
    ; Call the C main function
    call main
    
    ; If main returns (shouldn't), hang
.hang:
    hlt
    jmp .hang

