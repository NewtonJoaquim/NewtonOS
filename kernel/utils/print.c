void itoa(int value, char* str) {
    char buf[32];
    int i = 0, j = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    int is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    while (value > 0) {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (is_negative) buf[i++] = '-';

    while (i > 0) str[j++] = buf[--i];
    str[j] = '\0';
}

// Very simple formatter: supports %d and %s in order
int sprintf(char* out, const char* fmt,
            int arg1, int arg2, int arg3,
            int arg4, int arg5, int arg6,
            const char* str1, const char* str2) {
    char* p = out;
    const char* f = fmt;

    int args[6] = {arg1, arg2, arg3, arg4, arg5, arg6};
    int arg_index = 0;
    const char* strs[2] = {str1, str2};
    int str_index = 0;

    while (*f) {
        if (*f == '%') {
            f++;
            if (*f == 'd') {
                // integer
                char buf[32];
                itoa(args[arg_index++], buf);
                for (char* b = buf; *b; b++) *p++ = *b;
            } else if (*f == 's') {
                // string
                const char* s = strs[str_index++];
                while (s && *s) *p++ = *s++;
            } else {
                *p++ = '%';
                *p++ = *f;
            }
        } else {
            *p++ = *f;
        }
        f++;
    }
    *p = '\0';
    return (p - out);
}