// kernel.c
// A tiny C kernel that prints a message using BIOS interrupts

void print(const char* str) {
    while (*str) {
        // BIOS teletype output (int 0x10, ah=0x0E, al=character)
        asm volatile (
            "movb $0x0E, %%ah\n\t"
            "movb %0, %%al\n\t"
            "int $0x10"
            :
            : "r"((unsigned char)*str)
            : "ax"
        );
        str++;
    }
}

void main() {
    print("Hello from C kernel!\r\n");
    for (;;); // hang
}
