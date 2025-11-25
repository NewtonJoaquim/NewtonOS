#include "shell.h"
#include "string.h"

void shell(void) {
    vga_clear();
    vga_print_colorln("NewtonOS Shell", VGA_LIGHT_GREEN, VGA_BLACK);
    vga_print_colorln("Type 'help' for commands.", VGA_CYAN, VGA_BLACK);

    while (1) {
        vga_print_color("> ", VGA_YELLOW, VGA_BLACK); // prompt

        // Wait until a line is ready
        char* line = NULL;
        while ((line = keyboard_readline()) == NULL) {
            // spin until Enter pressed
        }

        // Process command
        if (strcmp(line, "help") == 0) {
            vga_println("Available commands:");
            vga_println("  help  - show this message");
            vga_println("  clear - clear the screen");
            vga_println("  echo  - repeat your input");
        } else if (strcmp(line, "clear") == 0) {
            vga_clear();
        } else if (strncmp(line, "echo ", 5) == 0) {
            vga_println(line + 5);
        } else {
            vga_print("Unknown command: ");
            vga_println(line);
        }
    }
}
