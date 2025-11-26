#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_BUFFER_SIZE 128
#define NULL 0

#define KEY_UP    0x100
#define KEY_DOWN  0x101
#define KEY_LEFT  0x102
#define KEY_RIGHT 0x103

#include <stdint.h>
#include "scancode_table.c"
#include "io_helpers.h"
#include "vga/vga_helpers.h"

static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_index = 0;
static int line_ready = 0;
static int last_key = 0;

void keyboard_handler(void);
char translate_scancode(uint8_t scancode);
char* keyboard_readline(void);
char* keyboard_readline(void);
int keyboard_last_key(void);

#endif