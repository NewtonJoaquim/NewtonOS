#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
void strlcpy(char* dest, const char* src, int max);
void str_trim_newline(char* s);
size_t strlen(const char* s);

#endif
