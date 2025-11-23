#include "keyboard.h"

static int shift_pressed = 0;
static int capslock_pressed = 0;

void keyboard_handler(void)
{
    uint8_t scancode = inb(0x60); // read from keyboard port

    // Handle Shift press/release
    if (scancode == 0x2A || scancode == 0x36)
    {
        shift_pressed = 1;
        outb(0x20, 0x20);
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6)
    {
        shift_pressed = 0;
        outb(0x20, 0x20);
        return;
    }

    // Handle capslock press (make code is 0x3A)
    if (scancode == 0x3A)
    {
        capslock_pressed = !capslock_pressed;
        outb(0x20, 0x20);
        return;
    }

    // Ignore key releases
    if (scancode & 0x80)
    {
        outb(0x20, 0x20); // EOI
        return;
    }

    char c = translate_scancode(scancode); // simple lookup
    if (c)
    {
        if (c == '\n')
        {
            keyboard_buffer[buffer_index] = '\0';
            line_ready = 1;
            buffer_index = 0;
            vga_put_char('\n');
        }
        else if (c == '\b')
        {
            if(buffer_index > 0) buffer_index--;
            vga_backspace();
        }
        else
        {
            if(buffer_index < KEYBOARD_BUFFER_SIZE - 1){
                keyboard_buffer[buffer_index] = c;
                buffer_index++;
                vga_put_char(c);
            }
        }
    }

    outb(0x20, 0x20); // send EOI to PIC
}

char translate_scancode(uint8_t scancode)
{
    if (scancode < 128)
    {
        if (shift_pressed && capslock_pressed)
            return scancode_table_shift_caps[scancode];
        else if (shift_pressed)
            return scancode_table_shift[scancode];
        else if (capslock_pressed)
            return scancode_table_caps[scancode];
        else
            return scancode_table[scancode];
    }
    return 0;
}
