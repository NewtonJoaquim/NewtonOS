// vga_helpers.h
#ifndef VGA_HELPERS_H
#define VGA_HELPERS_H

#include <stdint.h>

#define COLS  80
#define ROWS 25
#define VGA_ADDRESS 0xB8000

void clear_screen(void);
#define vga_clear clear_screen
void vga_put_char(char c);
void vga_print(char* str);
void vga_scroll(void);
void vga_print_hex(uint8_t byte);
void vga_backspace(void);
void vga_println(const char* str);


#endif
