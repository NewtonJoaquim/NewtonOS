#include "shell.h"
#include "string.h"
#include "info.h"
#include "file_system/fs.h"

#define HISTORY_SIZE 10
#define MAX_CMD_LEN 128

static char history[HISTORY_SIZE][MAX_CMD_LEN];
static int history_count = 0;
static int history_index = -1; // -1 means no history selected

void add_history(const char *line)
{
    if (strlen(line) > 0)
    {
        strlcpy(history[history_count % HISTORY_SIZE], line, MAX_CMD_LEN);
        history_count++;
        history_index = -1; // reset navigation
    }
}

void recall_previous_command(char *buffer, int *buffer_index)
{
    if (history_count > 0)
    {
        if (history_index < history_count - 1)
        {
            history_index++;
        }
        strlcpy(buffer, history[(history_count - 1 - history_index) % HISTORY_SIZE], MAX_CMD_LEN);
        *buffer_index = strlen(buffer);

        vga_print(buffer);
    }
}

void recall_next_command(char *buffer, int *buffer_index)
{
    if (history_index > 0)
    {
        history_index--;
        strlcpy(buffer, history[(history_count - 1 - history_index) % HISTORY_SIZE], MAX_CMD_LEN);
        *buffer_index = strlen(buffer);

        vga_print(buffer);
    }
    else
    {
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

    init_files();
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

        str_trim_newline(line);
        add_history(line);

        // Process command
        if (strcmp(line, "help") == 0)
        {
            vga_println("Available commands:");
            vga_println("  help  - show this message");
            vga_println("  clear - clear the screen");
            vga_println("  echo  - repeat your input");
            vga_println("  ver   - show system info");
            vga_println("  date   - show date and time");
            vga_println("  uptime   - show system uptime");
            vga_println("  ls       - list files (stub)");
            vga_println("  cat      - show file contents (stub)");
            vga_println("  touch    - create a new file (stub)");
            vga_println("  rm       - delete a file (stub)");
        }
        else if (strcmp(line, "clear") == 0)
        {
            vga_clear();
        }
        else if (strncmp(line, "echo ", 5) == 0)
        {
            vga_println(line + 5);
        }
        else if (strcmp(line, "ver") == 0 || strcmp(line, "version") == 0)
        {
            vga_print_color(OS_NAME, VGA_LIGHT_GREEN, VGA_BLACK);
            vga_print(" ");
            vga_println(OS_VERSION);

            vga_print_color("Build: ", VGA_CYAN, VGA_BLACK);
            vga_println(OS_BUILD);
        }
        else if (strcmp(line, "date") == 0)
        {
            struct rtc_time now = get_time();
            char buf[64];
            int i = 0;
            // DD/MM/YYYY HH:MM:SS
            buf[i++] = '0' + (now.day / 10) % 10;
            buf[i++] = '0' + now.day % 10;
            buf[i++] = '/';
            buf[i++] = '0' + (now.month / 10) % 10;
            buf[i++] = '0' + now.month % 10;
            buf[i++] = '/';
            buf[i++] = '0' + (now.year / 1000) % 10;
            buf[i++] = '0' + (now.year / 100) % 10;
            buf[i++] = '0' + (now.year / 10) % 10;
            buf[i++] = '0' + now.year % 10;
            buf[i++] = ' ';
            buf[i++] = '0' + (now.hour / 10) % 10;
            buf[i++] = '0' + now.hour % 10;
            buf[i++] = ':';
            buf[i++] = '0' + (now.min / 10) % 10;
            buf[i++] = '0' + now.min % 10;
            buf[i++] = ':';
            buf[i++] = '0' + (now.sec / 10) % 10;
            buf[i++] = '0' + now.sec % 10;
            buf[i] = '\0';
            vga_println(buf);
        }
        else if (strcmp(line, "uptime") == 0)
        {
            struct uptime up = get_uptime();
            char buf[64];
            int i = 0;
            buf[i++] = '0' + (up.hours / 10) % 10;
            buf[i++] = '0' + up.hours % 10;
            buf[i++] = ':';
            buf[i++] = '0' + (up.minutes / 10) % 10;
            buf[i++] = '0' + up.minutes % 10;
            buf[i++] = ':';
            buf[i++] = '0' + (up.seconds / 10) % 10;
            buf[i++] = '0' + up.seconds % 10;
            buf[i] = '\0';
            vga_println(buf);
        }
        else if (strcmp(line, "ls") == 0)
        {
            vga_println("kernel.bin");
            vga_println("readme.txt");
            vga_println("shell.c");
            vga_println("drivers/");
        }
        else if (line[0] == 'c' && line[1] == 'a' && line[2] == 't' && line[3] == ' ')
        {
            char *filename = line + 4;
            if (strcmp(filename, "readme.txt") == 0)
            {
                vga_println("Welcome to NewtonOS!");
                vga_println("This is a stubbed file system.");
            }
            else if (strcmp(filename, "kernel.bin") == 0)
            {
                vga_println("Binary file: kernel.bin (stub)");
            }
            else if (strcmp(filename, "shell.c") == 0)
            {
                vga_println("// shell.c source code stub");
            }
            else
            {
                vga_print_color("File not found: ", VGA_LIGHT_RED, VGA_BLACK);
                vga_println(filename);
            }
        }
        else if (line[0] == 't' && line[1] == 'o' && line[2] == 'u' && line[3] == 'c' && line[4] == 'h' && line[5] == ' ')
        {
            char *filename = line + 6;

            if (file_count < MAX_FILES)
            {
                strlcpy(files[file_count], filename, MAX_FILENAME);
                file_count++;
                vga_print_color("Created file: ", VGA_LIGHT_GREEN, VGA_BLACK);
                vga_println(filename);
            }
            else
            {
                vga_print_color("File list full, cannot create: ", VGA_LIGHT_RED, VGA_BLACK);
                vga_println(filename);
            }
        }
        else if (line[0] == 'r' && line[1] == 'm' && line[2] == ' ')
        {
            char *filename = line + 3;
            int found = -1;

            // Search for file
            for (int i = 0; i < file_count; i++)
            {
                if (strcmp(files[i], filename) == 0)
                {
                    found = i;
                    break;
                }
            }

            if (found >= 0)
            {
                // Shift files down to remove entry
                for (int j = found; j < file_count - 1; j++)
                {
                    strlcpy(files[j], files[j + 1], MAX_FILENAME);
                }
                file_count--;

                vga_print_color("Deleted file: ", VGA_LIGHT_GREEN, VGA_BLACK);
                vga_println(filename);
            }
            else
            {
                vga_print_color("File not found: ", VGA_LIGHT_RED, VGA_BLACK);
                vga_println(filename);
            }
        }
        else
        {
            vga_print("Unknown command: ");
            vga_println(line);
        }
    }
}
