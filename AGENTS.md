# NewtonOS Agent Instructions

This file provides concise guidance for AI coding agents working on the NewtonOS kernel project.  It focuses on build commands, architecture, key directories, and common pitfalls.  All references point to existing documentation in the repository.

## Build & Run

- `make` – Build the floppy image `os.img` (bootloader + kernel).
- `make run` – Launch QEMU with the built image.
- `make clean` – Remove all generated files.

## Project Structure

- **boot/** – 16‑bit bootloader (`bootloader.asm`) and 32‑bit kernel entry (`kernel_entry.asm`).
- **kernel/** – Main C kernel and subsystems.
  - `main.c` – Kernel entry point, initializes hardware and starts the shell.
  - `drivers/` – Hardware drivers (VGA, keyboard, timer, RTC).
  - `idt/` – Interrupt handling (IDT, PIC remap, ISR stubs).
  - `shell/` – User‑facing shell.
  - `file_system/` – Basic file system implementation.
  - `utils/` – String and I/O helpers.
- **build/** – Compiled output (bootloader, kernel, `os.img`).
- **linker.ld** – Linker script; defines memory layout starting at `0x1000`.
- **Makefile** – Build orchestration.

## Key Conventions

- **Assembler**: `nasm` (bootloader) and `nasm -f elf32` (kernel objects).
- **C Compiler**: `i386-elf-gcc` with `-ffreestanding -m32 -O2 -nostdlib`.
- **Linker**: `i386-elf-ld` with `linker.ld`.
- **Memory Model**: Flat 32‑bit, no paging.
- **Segment Selectors**: Code `0x08`, Data `0x10`, Stack `0x18`.
- **Interrupts**: 256 IDT entries, PIC remapped to `0x20`/`0x28`.
- **VGA**: Direct writes to `0xB8000` (80×25 text mode, 16 colors).
- **Port I/O**: Helpers in `utils/io_helpers.h`.

## Common Pitfalls

- **Kernel size**: Must fit within the 1.44 MB floppy image; otherwise `make` will fail.
- **Bootloader assumptions**: Kernel must be contiguous starting at LBA 1.
- **Linker script**: `linker.ld` must match the bootloader load address (`0x1000`).
- **No paging**: All addresses are physical; changing the memory layout requires updating the linker script and bootloader.
- **GDT**: Set up only in the bootloader; kernel assumes a minimal GDT.

## Useful Links

- [linker.ld](linker.ld) – Memory layout contract.
- [boot/bootloader.asm](boot/bootloader.asm) – Boot protocol.
- [kernel/main.c](kernel/main.c) – Kernel initialization.
- [kernel/idt/idt.c](kernel/idt/idt.c) – IDT setup.
- [kernel/drivers/keyboard/keyboard.c](kernel/drivers/keyboard/keyboard.c) – Keyboard driver.
- [kernel/drivers/timer/timer.c](kernel/drivers/timer/timer.c) – PIT timer.
- [kernel/drivers/real_time_clock/rtc.c](kernel/drivers/real_time_clock/rtc.c) – RTC driver.
- [kernel/drivers/vga/vga_helpers.c](kernel/drivers/vga/vga_helpers.c) – VGA helpers.
- [kernel/file_system/fs.c](kernel/file_system/fs.c) – File system.
- [kernel/shell/shell.c](kernel/shell/shell.c) – Shell implementation.

Feel free to ask for more details or clarification on any part of the project.