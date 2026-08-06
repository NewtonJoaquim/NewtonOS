ASM      = nasm
ASMFLAGS = -f bin
ASMFLAGS_ELF = -f elf32

CC       = i386-elf-gcc
CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -nostdlib -Ikernel -Ikernel/utils -Ikernel/drivers
LD       = i386-elf-ld
LDFLAGS  = -T linker.ld
OBJCOPY  = i386-elf-objcopy

QEMU     = qemu-system-i386

BOOT     = boot/bootloader.asm
BOOTBIN  = build/bootloader.bin
KERNELENTRY = boot/kernel_entry.asm
KERNELENTRYO = build/kernel_entry.o
KERNELC  = kernel/main.c
KERNELO  = build/kernel.o
KERNELELF= build/kernel.elf
KERNELBIN= build/kernel.bin
IMAGE    = build/os.img
ISRSTUBS    = kernel/idt/isr_stubs.asm
ISRSTUBSO   = build/isr_stubs.o
IDT       = kernel/idt/idt.c
IDTO      = build/idt.o
PICREMAP  = kernel/idt/pic_remap.c
PICREMAPO = build/pic_remap.o
VGA       = kernel/drivers/vga/vga_helpers.c
VGAO      = build/vga_helpers.o
KEYBOARD    = kernel/drivers/keyboard/keyboard.c
KEYBOARDO   = build/keyboard.o
RTC         = kernel/drivers/real_time_clock/rtc.c
RTCO        = build/rtc.o
SHELLSRC    = kernel/shell/shell.c
SHELLO      = build/shell.o
STRING      = kernel/utils/string.c
STRINGO     = build/string.o
TIMER       = kernel/drivers/timer/timer.c
TIMERO      = build/timer.o
FS       = kernel/file_system/fs.c
FSO      = build/fs.o

BUILD_DIR   = build
FLOPPY_SIZE = 1474560

all: $(BUILD_DIR) $(IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOTBIN): $(BOOT) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(KERNELENTRYO): $(KERNELENTRY) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(KERNELO): $(KERNELC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ISRSTUBSO): $(ISRSTUBS) | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(IDTO): $(IDT) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(FSO): $(FS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(PICREMAPO): $(PICREMAP) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(VGAO): $(VGA) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KEYBOARDO): $(KEYBOARD) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(RTCO): $(RTC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SHELLO): $(SHELLSRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(STRINGO): $(STRING) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TIMERO): $(TIMER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNELELF): $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(VGAO) $(KEYBOARDO) $(RTCO) $(SHELLO) $(STRINGO) $(TIMERO) $(FSO) linker.ld | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(VGAO) $(KEYBOARDO) $(RTCO) $(SHELLO) $(STRINGO) $(TIMERO) $(FSO)

$(KERNELBIN): $(KERNELELF) | $(BUILD_DIR)
	$(OBJCOPY) -O binary $< $@

$(IMAGE): $(BOOTBIN) $(KERNELBIN) | $(BUILD_DIR)
	cat $(BOOTBIN) $(KERNELBIN) > $(IMAGE)
	truncate -s $(FLOPPY_SIZE) $(IMAGE)

run: $(IMAGE)
	$(QEMU) -fda $(IMAGE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run