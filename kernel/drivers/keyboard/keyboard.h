#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_BUFFER_SIZE 128

#define KEY_UP    0x100
#define KEY_DOWN  0x101
#define KEY_LEFT  0x102
#define KEY_RIGHT 0x103

#include "io_helpers.h"

extern char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
extern int buffer_index;
extern int line_ready;
extern int last_key;

void keyboard_handler(void);
char translate_scancode(uint8_t scancode);
char* keyboard_readline(void);
int keyboard_last_key(void);

#endif