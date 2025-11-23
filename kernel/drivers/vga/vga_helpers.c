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
static int cursor_x = 0;
static int cursor_y = 0;

int get_cursor_position(void){
    return cursor_y * COLS + cursor_x;
}

//writes a char to the vga buffer
void vga_put_char(char c){
    if(c == '\n'){
        cursor_x = 0;
        cursor_y++;
    } else if(c == '\b') {
        if(cursor_x > 0) cursor_x--;
        else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = COLS - 1;
        }
        VGA_BUFFER[get_cursor_position()] = (uint16_t)' ' | (0x07 << 8);
    } else{
        VGA_BUFFER[get_cursor_position()] = (uint16_t)c | (0x07 << 8);
        cursor_x++;
        if(cursor_x >= COLS) {
            vga_newline();
        }
    }
    //check if we need to scroll
    if(cursor_y >= ROWS) {
        vga_scroll();
        cursor_y = ROWS - 1;
    }
    
    vga_update_cursor();   
}
    
void vga_newline(void){
    cursor_x = 0;
    cursor_y++;

     // Optional: skip empty rows
    int row_start = cursor_y * COLS;
    int row_end   = row_start + COLS;

    int last_filled = -1;
    for (int i = row_end - 1; i >= row_start; i--) {
        char ch = (char)(VGA_BUFFER[i] & 0xFF);
        if (ch != ' ') {
            last_filled = i - row_start;
            break;
        }
    }

    if (last_filled != -1) {
        // If row already has text, move cursor after it
        cursor_x = last_filled + 1;
        if (cursor_x >= COLS) {
            cursor_x = 0;
            cursor_y++;
        }
    } else {
        // Row is empty → stay at column 0
        cursor_x = 0;
    }

    vga_update_cursor();
}

void vga_print(char* str){
    while(*str) {
        vga_put_char(*str);
        *str++;
    }
}

void vga_println(const char* str) {
    vga_print(str);
    vga_newline();
}


void vga_scroll(void){
    for(int i = COLS; i< COLS * ROWS; i++){
        VGA_BUFFER[i - COLS] = VGA_BUFFER[i];
    }
    for(int i = COLS * (ROWS - 1) ; i < COLS * ROWS; i++){
        VGA_BUFFER[i] = ' ' | (0x07 << 8);
    }
}

void vga_print_hex(uint8_t byte) {
    const char* hex_chars = "0123456789ABCDEF";

    vga_put_char('0');
    vga_put_char('x');
    vga_put_char(hex_chars[(byte >> 4) & 0x0F]); // high nibble
    vga_put_char(hex_chars[byte & 0x0F]);        // low nibble
}

void vga_update_cursor(void) {
    int pos = get_cursor_position();
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}


void vga_backspace(void) {
    if (cursor_x > 0) {
        cursor_x--; // move back one cell
    }
    else if(cursor_y > 0){
        cursor_y--;
        //cursor_x = COLS - 1;
        int row_start = cursor_y * COLS;
        int row_end   = row_start + COLS;

        int last_filled = -1;
        for(int i = row_end-1 ; i >= row_start; i--){
            char ch = (char)(VGA_BUFFER[i] & 0xFF);
            if(ch != ' '){
                last_filled = i - row_start;
                break;
            }
        }
        if(last_filled != -1){
            cursor_x = last_filled;
        } else {
            cursor_x = 0;
        }

    }
    VGA_BUFFER[get_cursor_position()] = (uint16_t)' ' | (0x07 << 8); // overwrite with space
    vga_update_cursor(); // update hardware cursor
}


void clear_screen(){
    for (int i = 0; i < COLS * ROWS; i++) {
        VGA_BUFFER[i] = (unsigned short)((0x07 << 8) | ' ');
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_update_cursor();
}
