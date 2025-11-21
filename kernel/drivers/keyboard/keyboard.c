#include "keyboard.h"

static int shift_pressed = 0;

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);   // read from keyboard port
    
    // Handle Shift press/release
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        outb(0x20, 0x20);
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        outb(0x20, 0x20);
        return;
    }


    // Ignore key releases
    if (scancode & 0x80) {
        outb(0x20, 0x20); // EOI
        return;
    }

    char c = translate_scancode(scancode); // simple lookup
     if (c) {
        vga_put_char(c);
    }

    
    outb(0x20, 0x20);               // send EOI to PIC
}

char translate_scancode(uint8_t scancode) {
    if (scancode < 128) {
        if (shift_pressed)
            return scancode_table_shift[scancode];
        else
            return scancode_table[scancode];
    }
    return 0;

}
