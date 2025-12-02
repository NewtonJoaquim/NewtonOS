#define MAX_FILES 20
#define MAX_FILENAME 32

#include "string.h"

static char files[MAX_FILES][MAX_FILENAME];
static int file_count = 3;

void init_files(){
    strlcpy(files[0], "kernel.bin", MAX_FILENAME);
    strlcpy(files[1], "readme.txt", MAX_FILENAME);
    strlcpy(files[2], "shell.c", MAX_FILENAME);
}