#include "shell.h"
#include "string.h"

#define HISTORY_SIZE 10
#define MAX_CMD_LEN 128

static char history[HISTORY_SIZE][MAX_CMD_LEN];
static int history_count = 0;
static int history_index = -1; // -1 means no history selected

void add_history(const char* line) {
    if (strlen(line) > 0) {
        strcpy(history[history_count % HISTORY_SIZE], line, MAX_CMD_LEN);
        history_count++;
        history_index = -1; // reset navigation
    }
}

void recall_previous_command(char* buffer, int* buffer_index) {
    if (history_count > 0) {
        if (history_index < history_count - 1) {
            history_index++;
        }
        strcpy(buffer, history[(history_count - 1 - history_index) % HISTORY_SIZE], MAX_CMD_LEN);
        *buffer_index = strlen(buffer);

        vga_print(buffer);
    }
}

void recall_next_command(char* buffer, int* buffer_index) {
    if (history_index > 0) {
        history_index--;
        strcpy(buffer, history[(history_count - 1 - history_index) % HISTORY_SIZE], MAX_CMD_LEN);
        *buffer_index = strlen(buffer);

        vga_print(buffer);
    } else {
        history_index = -1;
        buffer[0] = '\0';
        *buffer_index = 0;

        vga_print("\r> ");
    }
}


void shell(void)
{
    vga_clear();
    vga_print_colorln("NewtonOS Shell", VGA_LIGHT_GREEN, VGA_BLACK);
    vga_print_colorln("Type 'help' for commands.", VGA_CYAN, VGA_BLACK);

    while (1)
    {
        vga_print_color("> ", VGA_YELLOW, VGA_BLACK); // prompt
        buffer_index = 0;
        keyboard_buffer[0] = 0;

        // Wait until a line is ready
        char *line = NULL;
        while ((line = keyboard_readline()) == NULL)
        {
            int key = keyboard_last_key();
            if (key == KEY_UP)
            {
                recall_previous_command(keyboard_buffer, &buffer_index);
            }
            else if (key == KEY_DOWN)
            {
                recall_next_command(keyboard_buffer, &buffer_index);
            }
        }

        add_history(line);

        // Process command
        if (strcmp(line, "help") == 0)
        {
            vga_println("Available commands:");
            vga_println("  help  - show this message");
            vga_println("  clear - clear the screen");
            vga_println("  echo  - repeat your input");
        }
        else if (strcmp(line, "clear") == 0)
        {
            vga_clear();
        }
        else if (strncmp(line, "echo ", 5) == 0)
        {
            vga_println(line + 5);
        }
        else
        {
            vga_print("Unknown command: ");
            vga_println(line);
        }
    }
}
