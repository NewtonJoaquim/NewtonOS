#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "scancode_table.c"
#include "io_helpers.h"

void keyboard_handler(void);
char translate_scancode(uint8_t scancode);

#endif