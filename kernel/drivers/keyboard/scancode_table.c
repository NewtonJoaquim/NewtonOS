#include <stdint.h>

static const char scancode_table[128] = {
    0,   27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // 0x0E = Backspace
    '\t', // 0x0F = Tab
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',            // 0x1C = Enter
    0,    // 0x1D = Ctrl
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,    // 0x2A = Left Shift
    '\\','z','x','c','v','b','n','m',',','.','/',
    0,    // 0x36 = Right Shift
    '*',
    0,    // Alt
    ' ',  // Space bar
    // … fill in more if needed
};
