ASM      = nasm
ASMFLAGS = -f bin
ASMFLAGS_ELF = -f elf32

CC       = i386-elf-gcc
CFLAGS   = -ffreestanding -m32 -O2 -Wall -Wextra -nostdlib
LD       = i386-elf-ld
LDFLAGS  = -T linker.ld
OBJCOPY  = i386-elf-objcopy

QEMU     = qemu-system-i386

BOOT     = boot/bootloader.asm
BOOTBIN  = build/bootloader.bin
KERNELENTRY = boot/kernel_entry.asm
KERNELENTRYO = build/kernel_entry.o
KERNELC  = kernel/kernel.c
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
VGA       = kernel/drivers/vga_helpers.c
VGAO      = build/vga_helpers.o


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

$(KERNELELF): $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(VGAO) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(VGAO)

$(KERNELBIN): $(KERNELELF)
	$(OBJCOPY) -O binary $< $@

$(IMAGE): $(BOOTBIN) $(KERNELBIN)
	cat $(BOOTBIN) $(KERNELBIN) > $(IMAGE)
	truncate -s $(FLOPPY_SIZE) $(IMAGE)

run: $(IMAGE)
	$(QEMU) -fda $(IMAGE)

clean:
	rm -f $(BOOTBIN) $(KERNELENTRYO) $(KERNELO) $(ISRSTUBSO) $(IDTO) $(PICREMAPO) $(KERNELELF) $(KERNELBIN) $(IMAGE)