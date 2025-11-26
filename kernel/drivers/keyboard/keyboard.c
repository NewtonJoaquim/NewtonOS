#include "keyboard.h"

static int shift_pressed = 0;
static int capslock_pressed = 0;

void keyboard_handler(void)
{
    uint8_t scancode = inb(0x60); // read from keyboard port

    if (scancode == 0xE0)
    {
        last_key = scancode;
        outb(0x20, 0x20);
        return;
    }

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

    if (last_key == 0xE0)
    {
        // extended key sequence
        switch (scancode)
        {
        case 0x48:
            last_key = KEY_UP;
            break;
        case 0x50:
            last_key = KEY_DOWN;
            break;
        case 0x4B:
            last_key = KEY_LEFT;
            break;
        case 0x4D:
            last_key = KEY_RIGHT;
            break;
        default:
            last_key = 0;
            break;
        }
    }
    else
    {
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
                if (buffer_index > 0)
                    buffer_index--;
                vga_backspace();
            }
            else
            {
                if (buffer_index < KEYBOARD_BUFFER_SIZE - 1)
                {
                    keyboard_buffer[buffer_index] = c;
                    buffer_index++;
                    vga_put_char(c);
                }
            }
        }
    }
    outb(0x20, 0x20); // send EOI to PIC
}

char *keyboard_readline(void)
{
    if (line_ready)
    {
        line_ready = 0; // reset flag
        return keyboard_buffer;
    }
    return NULL; // no line yet
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

int keyboard_last_key(void)
{
    int key = last_key;
    last_key = 0; // clear after read
    return key;
}
