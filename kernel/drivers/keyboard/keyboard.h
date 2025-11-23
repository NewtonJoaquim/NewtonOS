#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_BUFFER_SIZE 128

#include <stdint.h>
#include "scancode_table.c"
#include "io_helpers.h"
#include "vga/vga_helpers.h"

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_index = 0;
static int line_ready = 0;

void keyboard_handler(void);
char translate_scancode(uint8_t scancode);

#endif