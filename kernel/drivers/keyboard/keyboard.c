#include "keyboard.h"

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);   // read from keyboard port
    char c = translate_scancode(scancode); // simple lookup
    vga_put_char(c);
    outb(0x20, 0x20);               // send EOI to PIC
}

char translate_scancode(uint8_t scancode) {
    if (scancode < sizeof(scancode_table)) {
        return scancode_table[scancode];
    }
    return 0; // unknown key
}
