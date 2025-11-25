#include "drivers/vga/vga_helpers.h"
#include "idt/idt.h"
#include "idt/pic_remap.h"
#include "drivers/keyboard/keyboard.h"
#include "shell/shell.h"

// kernel.c
// A tiny C kernel that prints a message using VGA text

void main() {
    clear_screen();
    // Step 1: Remap PIC
    pic_remap();

    // Step 2: Initialize IDT (including keyboard)
    idt_init();

    // Step 4: Enable interrupts
    asm volatile("sti");

    vga_println("Welcome to NewtonOS!");
    vga_println("Type 'help' for commands.");
    shell();

    // Halt loop
    for (;;) {
        asm volatile("hlt");
    }

}
