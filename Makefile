# Toolchain
AS = nasm
CC = clang
LD = ld

# Output image name
IMAGE_NAME = image

# Kernel source files 
KERNEL_ASM_SOURCES = $(shell cd source && find -L * -type f -name '*.asm')
KERNEL_C_SOURCES = $(shell cd source && find -L * -type f -name '*.c')

# Object files
KERNEL_OBJS = $(addprefix binaries/kernel/, $(KERNEL_ASM_SOURCES:.asm=.asm.o) $(KERNEL_C_SOURCES:.c=.c.o))

# Flags
ASFLAGS = -f elf64 -g -F dwarf
CCFLAGS = -m64 -std=gnu11 -ffreestanding -Wall -Wextra -nostdlib -I source -fno-stack-protector -Wno-unused-parameter -fno-stack-check -fno-lto -mno-red-zone
QEMUFLAGS = -debugcon stdio -cdrom binaries/$(IMAGE_NAME).iso -boot d
LDFLAGS = -m elf_x86_64 -Tsource/linker.ld -z noexecstack

all:  kernel iso

run: all
	@qemu-system-x86_64 $(QEMUFLAGS)

run-gdb: all
	@qemu-system-x86_64 $(QEMUFLAGS) -S -s

binaries/kernel/%.c.o: source/%.c
	@echo " clang $<"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CCFLAGS) -c $< -o $@

binaries/kernel/%.asm.o: source/%.asm
	@echo " assembly $<"
	@mkdir -p "$$(dirname $@)"
	@$(AS) $(ASFLAGS) -o $@ $<

kernel: $(KERNEL_OBJS)
	@echo " linking source/*"
	@$(LD) $(LDFLAGS) $^ -o binaries/kernel.elf

iso:
	@grub-file --is-x86-multiboot2 ./binaries/kernel.elf; \
	if [ $$? -eq 1 ]; then \
		echo " error: kernel.elf is not a valid multiboot2 file"; \
		exit 1; \
	fi
	@mkdir -p iso_root/boot/grub/
	@cp binaries/kernel.elf iso_root/boot/kernel.elf
	@cp bootloader/grub.cfg iso_root/boot/grub/grub.cfg
	@grub-mkrescue -o binaries/$(IMAGE_NAME).iso iso_root/ -quiet 2>&1 >/dev/null | grep -v libburnia | cat
	@rm -rf iso_root/

clean:
	@rm -f $(KERNEL_OBJS)
	@rm -rf binaries
	@rm -f binaries/$(IMAGE_NAME).iso
