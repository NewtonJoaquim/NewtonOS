; bootloader.asm
; Loads N sectors right after the boot sector into memory and jumps to them.
; Assumptions:
; - BIOS loads this at 0x7C00 in 16-bit real mode.
; - The kernel is contiguous, starting at LBA=1 (sector immediately after boot).
; - We read SECTOR_COUNT sectors into 0000:1000 (physical 0x10000).
; - Kernel entry is at the load address.

BITS 16
ORG 0x7C00

in al, 0x92
or al, 00000010b
out 0x92, al

start:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode_entry   ; 0x08 = code segment selector

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00       ; simple stack
    sti

    ; Disk is in DL from BIOS (0x00 floppy, 0x80 HDD)
    ; Load to ES:BX = 0x0000:0x1000 (physical 0x10000)
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x1000

    ; CHS read using INT 13h AH=02h (works in BIOS without LBA extensions)
    ; Read SECTOR_COUNT sectors starting from LBA 1 -> CHS (0/0/2) on a floppy
    ; For HDD images, prefer INT 13h extensions (AH=42h). This is the simplest.

    ; Drive in DL already
    mov ah, 0x02          ; BIOS read sectors
    mov al, SECTOR_COUNT  ; number of sectors
    mov ch, 0x00          ; cylinder 0
    mov dh, 0x00          ; head 0
    mov cl, 0x02          ; sector 2 (sector numbering starts at 1; 1 is boot)
    int 0x13

    ; Success — jump to kernel at 0x0000:0x1000
    jmp 0x0000:0x1000

hang:
    jmp hang


[BITS 32]
protected_mode_entry:
    mov ax, 0x10        ; data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Now you’re in 32-bit protected mode!
    ; You can call your C kernel here
    call main

gdt_start:
    dq 0x0000000000000000   ; Null
    dq 0x00CF9A000000FFFF   ; Code segment
    dq 0x00CF92000000FFFF   ; Data segment
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start


SECTOR_COUNT equ 20         ; adjust to your kernel size in sectors

times 510-($-$$) db 0
dw 0xAA55