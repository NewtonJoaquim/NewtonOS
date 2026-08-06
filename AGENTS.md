# NewtonOS Agent Instructions

This file provides concise guidance for AI coding agents working on the NewtonOS kernel project. It focuses on build commands, architecture, key directories, coding conventions, and common pitfalls.

## Build & Toolchain

### Prerequisites
- **Assembler**: `nasm`
- **C Compiler**: `i386-elf-gcc` (freestanding cross-compiler target)
- **Linker**: `i386-elf-ld`
- **Binary Copy**: `i386-elf-objcopy`
- **Emulator**: `qemu-system-i386`

### Commands
- `make` – Compiles all modules, links kernel, and builds floppy disk image `build/os.img`.
- `make run` – Builds image (if needed) and launches `qemu-system-i386` with `-fda build/os.img`.
- `make clean` – Removes all compiled binaries and object files from `build/`.

## Project Structure

- **[boot/](boot/)** – Bootloader and kernel setup assembly files.
  - `bootloader.asm` – 16-bit real mode bootloader, enables A20 line, loads kernel sectors from disk to physical `0x1000`, enters 32-bit protected mode with GDT, jumps to `0x1000`.
  - `kernel_entry.asm` – 32-bit entry stub (`_start`), initializes stack pointer `esp` to `0x9000`, and calls C `main()`.
- **[kernel/](kernel/)** – Core C kernel modules and drivers.
  - `main.c` – Kernel initialization sequence (`initalize()`) and main execution loop (`shell()`).
  - **[drivers/](kernel/drivers/)** – Hardware interaction layer:
    - `vga/` – VGA text-mode buffer drivers and color constants (`0xB8000`).
    - `keyboard/` – PS/2 keyboard IRQ1 handler and scan-code mapping.
    - `timer/` – PIT (Programmable Interval Timer) IRQ0 handler.
    - `real_time_clock/` – CMOS RTC real-time clock driver.
  - **[idt/](kernel/idt/)** – Interrupt Descriptor Table management:
    - `idt.c` / `idt.h` – Gate setup and ISR vector table.
    - `pic_remap.c` / `pic_remap.h` – Remaps 8259 PIC vectors to `0x20` (master) and `0x28` (slave).
    - `isr_stubs.asm` – Low-level assembly ISR entry stubs.
  - **[shell/](kernel/shell/)** – Interactive CLI shell implementation.
  - **[file_system/](kernel/file_system/)** – Basic file system stub and interfaces.
  - **[memory/](kernel/memory/)** – Reserved directory for future physical/virtual memory management modules.
  - **[utils/](kernel/utils/)** – Helpers and hardware primitives:
    - `io_helpers.h` – Port I/O inline assembly (`inb`, `outb`).
    - `string.c` / `string.h` – Standard C string functions (`strlen`, `strcmp`, `strcpy`, `itoa`).
    - `info.h` – System version / branding metadata.
- **[build/](build/)** – Output directory for `.o`, `.bin`, `.elf`, and `os.img`.
- **[linker.ld](linker.ld)** – Linker script setting text section origin at `0x1000`.
- **[Makefile](Makefile)** – Build orchestration.

## Key Architecture & Conventions

- **Toolchain Flags**:
  - `CFLAGS`: `-ffreestanding -m32 -O2 -Wall -Wextra -nostdlib -Ikernel -Ikernel/utils -Ikernel/drivers`
  - Include resolution: Source files use relative include paths resolved via `-Ikernel`, `-Ikernel/utils`, and `-Ikernel/drivers`.
- **Memory Map**:
  - `0x7C00`: Bootloader loaded by BIOS.
  - `0x1000`: Kernel entry point (physical address specified in `linker.ld` and `bootloader.asm`).
  - `0x9000`: Kernel stack top pointer (`esp`).
  - `0xB8000`: VGA text mode frame buffer (80×25 text mode, 16 colors).
- **Segment Selectors**:
  - `0x08`: Code Segment (32-bit executable, flat 4GB limit).
  - `0x10`: Data Segment (32-bit read/write, flat 4GB limit).
- **Interrupt Vector Alignment**:
  - Hardware IRQs remapped to `0x20`..`0x2F` to avoid conflict with x86 CPU exception vectors `0x00`..`0x1F`.

## Common Pitfalls & Agent Checklist

- **Updating Makefile**: When adding a new `.c` or `.asm` file to the kernel, remember to add a rule to `Makefile` and include the object file in `KERNELELF` link list.
- **Sector Count Limit in Bootloader**: `boot/bootloader.asm` defines `SECTOR_COUNT equ 20` (loads 20 sectors = 10,240 bytes). If kernel binary (`build/kernel.bin`) grows beyond 10 KB, `SECTOR_COUNT` in `bootloader.asm` must be increased accordingly.
- **Freestanding Environment**: Standard C library headers (`<stdio.h>`, `<string.h>`, etc.) are unavailable due to `-nostdlib` and `-ffreestanding`. Always use custom helpers from `kernel/utils/`.
- **Memory Layout & Linker Contract**: `linker.ld` sets `. = 0x1000;`. The bootloader jumps directly to `0x1000`. `boot/kernel_entry.asm` must remain the first object passed to `i386-elf-ld` so that `_start` resides at offset `0x1000`.

## Useful Links

- [Makefile](Makefile) – Build configuration.
- [linker.ld](linker.ld) – Memory layout contract.
- [boot/bootloader.asm](boot/bootloader.asm) – Boot protocol & protected mode setup.
- [boot/kernel_entry.asm](boot/kernel_entry.asm) – Kernel entry stub & stack initialization.
- [kernel/main.c](kernel/main.c) – Main initialization flow.
- [kernel/idt/idt.c](kernel/idt/idt.c) – Interrupt descriptor table.
- [kernel/idt/isr_stubs.asm](kernel/idt/isr_stubs.asm) – Assembly ISR stubs.
- [kernel/drivers/keyboard/keyboard.c](kernel/drivers/keyboard/keyboard.c) – Keyboard driver.
- [kernel/drivers/timer/timer.c](kernel/drivers/timer/timer.c) – PIT timer driver.
- [kernel/drivers/real_time_clock/rtc.c](kernel/drivers/real_time_clock/rtc.c) – RTC driver.
- [kernel/drivers/vga/vga_helpers.c](kernel/drivers/vga/vga_helpers.c) – VGA screen handling.
- [kernel/shell/shell.c](kernel/shell/shell.c) – Command shell.
- [kernel/utils/io_helpers.h](kernel/utils/io_helpers.h) – Port I/O routines (`inb`/`outb`).
- [kernel/utils/string.h](kernel/utils/string.h) – Utility string declarations.