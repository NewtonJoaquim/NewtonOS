#include "vga_helpers.h"

/* 
 * VGA text mode buffer starts at physical address 0xB8000.
 * Each entry is 2 bytes: 
 *   - low byte = ASCII character
 *   - high byte = attribute (color: foreground + background)
 */
static uint16_t* const VGA_BUFFER = (uint16_t*)VGA_ADDRESS;

/* Cursor position: which cell we’re writing to (0..2000 for 80x25 screen). */
static int cursor = 0;

//writes a char to the vga buffer
void vga_put_char(char c){
    if(c == '\n'){
        cursor = COLS * ((cursor/COLS) + 1);
    } else if(c == '\r') {
        cursor = (cursor / COLS) * COLS;
    } else{
        VGA_BUFFER[cursor] = (uint16_t)c | (0x07 << 8);
        cursor++;
    }   
}

void vga_print(char* str){
    while(*str) {
        vga_put_char(*str);
        *str++;
    }
}

void vga_print_hex(uint8_t byte) {
    const char* hex_chars = "0123456789ABCDEF";

    vga_put_char('0');
    vga_put_char('x');
    vga_put_char(hex_chars[(byte >> 4) & 0x0F]); // high nibble
    vga_put_char(hex_chars[byte & 0x0F]);        // low nibble
}


void clear_screen(){
    for (int i = 0; i < COLS * ROWS; i++) {
        VGA_BUFFER[i] = (unsigned short)((0x07 << 8) | ' ');
    }
    cursor = 0;
}
