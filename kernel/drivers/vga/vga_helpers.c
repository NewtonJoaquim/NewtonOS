#include "vga_helpers.h"
#include "io_helpers.h"

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
        vga_scroll();
    } else if(c == '\r') {
        cursor = (cursor / COLS) * COLS;
    } else{
        VGA_BUFFER[cursor] = (uint16_t)c | (0x07 << 8);
        cursor++;
        vga_scroll();
    }
    vga_update_cursor(cursor);   
}

void vga_print(char* str){
    while(*str) {
        vga_put_char(*str);
        *str++;
    }
}

void vga_scroll(void){
    if(cursor >= COLS * ROWS ){
        //moving rows up
        for(int i=COLS; i<COLS*ROWS; i++) {
            VGA_BUFFER[i-COLS] = VGA_BUFFER[i];
        }
        //clear last line
        for(int i = COLS * (ROWS - 1); i <= COLS * ROWS; i++){
            VGA_BUFFER[i] = (uint16_t)' ' | (0x07 << 8);
        }
        //reset cursor to the beginning of last line
        cursor = COLS * (ROWS - 1);
        vga_update_cursor(cursor);
    }
}

void vga_print_hex(uint8_t byte) {
    const char* hex_chars = "0123456789ABCDEF";

    vga_put_char('0');
    vga_put_char('x');
    vga_put_char(hex_chars[(byte >> 4) & 0x0F]); // high nibble
    vga_put_char(hex_chars[byte & 0x0F]);        // low nibble
}

void vga_update_cursor(int pos) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}


void vga_backspace(void) {
    if (cursor > 0) {
        cursor--; // move back one cell
        VGA_BUFFER[cursor] = (uint16_t)' ' | (0x07 << 8); // overwrite with space
        vga_update_cursor(cursor); // update hardware cursor
    }
}


void clear_screen(){
    for (int i = 0; i < COLS * ROWS; i++) {
        VGA_BUFFER[i] = (unsigned short)((0x07 << 8) | ' ');
    }
    cursor = 0;
}
