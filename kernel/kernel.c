#include "drivers/vga/vga_helpers.h"
#include "idt/idt.h"
#include "idt/pic_remap.h"
#include "drivers/keyboard/keyboard.h"

// kernel.c
// A tiny C kernel that prints a message using VGA text

void main() {
    clear_screen();
    vga_print("Hello from C kernel!");
    vga_print("\nline 2!");

    // Step 1: Remap PIC
    pic_remap();

    // Step 2: Initialize IDT
    idt_init();

    // Step 4: Enable interrupts
    asm volatile("sti");

    // Halt loop
    for (;;) {
        asm volatile("hlt");
    }

}
