#include "drivers/vga/vga_helpers.h"
// kernel.c
// A tiny C kernel that prints a message using VGA text

void main() {
    clear_screen();
    vga_print("Hello from C kernel!");
    vga_print("\nline 2!");
    for (;;);
}
