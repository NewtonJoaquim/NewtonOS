ASM      = nasm
ASMFLAGS = -f bin
ASMFLAGS_ELF = -f elf32

CC       = i386-elf-gcc
CFLAGS   = -ffreestanding -m32 -O2 -Wall -Wextra -nostdlib
LD       = i386-elf-ld
LDFLAGS  = -T linker.ld
OBJCOPY  = i386-elf-objcopy

QEMU     = qemu-system-i386

BOOT     = bootloader.asm
BOOTBIN  = bootloader.bin
KERNELENTRY = kernel_entry.asm
KERNELENTRYO = kernel_entry.o
KERNELC  = kernel.c
KERNELO  = kernel.o
KERNELELF= kernel.elf
KERNELBIN= kernel.bin
IMAGE    = os.img

FLOPPY_SIZE = 1474560

all: $(IMAGE)

$(BOOTBIN): $(BOOT)
	$(ASM) $(ASMFLAGS) $< -o $@

$(KERNELENTRYO): $(KERNELENTRY)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

$(KERNELO): $(KERNELC)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNELELF): $(KERNELENTRYO) $(KERNELO) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(KERNELENTRYO) $(KERNELO)

$(KERNELBIN): $(KERNELELF)
	$(OBJCOPY) -O binary $< $@

$(IMAGE): $(BOOTBIN) $(KERNELBIN)
	cat $(BOOTBIN) $(KERNELBIN) > $(IMAGE)
	truncate -s $(FLOPPY_SIZE) $(IMAGE)

run: $(IMAGE)
	$(QEMU) -fda $(IMAGE)

clean:
	rm -f $(BOOTBIN) $(KERNELENTRYO) $(KERNELO) $(KERNELELF) $(KERNELBIN) $(IMAGE)