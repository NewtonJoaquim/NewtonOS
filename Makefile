ASM      = nasm
ASMFLAGS = -f bin
ASMFLAGS_ELF = -f elf32

CC       = i386-elf-gcc
CFLAGS = -ffreestanding -m32 -O2 -Wall -Wextra -nostdlib -Ikernel/utils -Ikernel/drivers
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


FLOPPY_SIZE = 1474560

all: $(IMAGE)

$(BOOTBIN): $(BOOT)
	$(ASM) $(ASMFLAGS) $< -o $@

$(KERNELENTRYO): $(KERNELENTRY)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(KERNELO): $(KERNELC)
	$(CC) $(CFLAGS) -c $< -o $@

$(ISRSTUBSO): $(ISRSTUBS)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(IDTO): $(IDT)
	$(CC) $(CFLAGS) -c $< -o $@

$(PICREMAPO): $(PICREMAP)
	$(CC) $(CFLAGS) -c $< -o $@

$(VGAO): $(VGA)
	$(CC) $(CFLAGS) -c $< -o $@

$(KEYBOARDO): $(KEYBOARD)
	$(CC) $(CFLAGS) -c $< -o $@

$(RTCO): $(RTC)
	$(CC) $(CFLAGS) -c $< -o $@

$(SHELLO): $(SHELLSRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(STRINGO): $(STRING)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNELELF): $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(VGAO) $(KEYBOARDO) $(RTCO) $(SHELLO) $(STRINGO) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(VGAO) $(KEYBOARDO) $(RTCO) $(SHELLO) $(STRINGO)

$(KERNELBIN): $(KERNELELF)
	$(OBJCOPY) -O binary $< $@

$(IMAGE): $(BOOTBIN) $(KERNELBIN)
	cat $(BOOTBIN) $(KERNELBIN) > $(IMAGE)
	truncate -s $(FLOPPY_SIZE) $(IMAGE)

run: $(IMAGE)
	$(QEMU) -fda $(IMAGE)

clean:
	rm -f $(BOOTBIN) $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(VGAO) $(KEYBOARDO) $(RTCO) $(SHELLO) $(STRINGO) $(KERNELELF) $(KERNELBIN) $(IMAGE)