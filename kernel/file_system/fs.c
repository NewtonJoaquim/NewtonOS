#include "fs.h"
#include "string.h"

void init_files(){
    strlcpy(files[0], "kernel.bin", MAX_FILENAME);
    strlcpy(files[1], "readme.txt", MAX_FILENAME);
    strlcpy(files[2], "shell.c", MAX_FILENAME);
}