// vga_helpers.h
#ifndef VGA_HELPERS_H
#define VGA_HELPERS_H

#include <stdint.h>

#define COLS  80
#define ROWS 25
#define VGA_ADDRESS 0xB8000


enum vga_color {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GREY = 7,
    VGA_DARK_GREY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_YELLOW = 14,
    VGA_WHITE = 15,
};

void clear_screen(void);
#define vga_clear clear_screen
void vga_put_char(char c);
void vga_print(char* str);
void vga_scroll(void);
void vga_print_hex(uint8_t byte);
void vga_backspace(void);
void vga_println(const char* str);
void vga_put_char_color(char c, enum vga_color fg, enum vga_color bg);
void vga_print_colorln(const char* str, enum vga_color fg, enum vga_color bg);
void vga_print_color(const char* str, enum vga_color fg, enum vga_color bg);

#endif
