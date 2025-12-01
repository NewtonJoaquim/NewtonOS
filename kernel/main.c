#include "drivers/vga/vga_helpers.h"
#include "idt/idt.h"
#include "idt/pic_remap.h"
#include "drivers/keyboard/keyboard.h"
#include "shell/shell.h"
#include "drivers/timer/timer.h"

// main.c
// A tiny C kernel with shell commands

void initalize(){
    clear_screen();
    // Step 1: Remap PIC
    pic_remap();

    // Step 2: Initialize IDT (including keyboard)
    idt_init();

    // Step 3: Initialize PIT timer
    timer_init();

    // Step 4: Enable interrupts
    asm volatile("sti");
}

void main() {
    initalize();
    shell();

    // Halt loop
    for (;;) {
        asm volatile("hlt");
    }

}
