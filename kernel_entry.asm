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
    
    ; Test: Write directly to VGA to verify kernel is executing
    mov edi, 0xB8000
    mov word [edi], 0x0F41      ; White 'A' on black
    mov word [edi+2], 0x0E42    ; Yellow 'B' on black
    mov word [edi+4], 0x0D43    ; Magenta 'C' on black
    
    ; Call the C main function
    call main
    
    ; If main returns (shouldn't), hang
.hang:
    hlt
    jmp .hang

