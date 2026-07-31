CC := gcc
LD := ld
OBJCOPY := objcopy
NASM := nasm
NASMFLAGS := -f elf64

KERNEL := build/kernel.elf
IMAGE := boot/sunix.img
IMAGE_SIZE_MB := 64

OVMF_CODE := /usr/share/edk2/x64/OVMF_CODE.4m.fd
OVMF_VARS := /usr/share/edk2/x64/OVMF_VARS.4m.fd

CFLAGS := -Iboot \
          -Ikernel \
          -Ibuild \
          -I. \
          -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector \
          -fno-stack-check -fno-lto -fno-pie -fno-pic \
          -m64 -march=x86-64 -mno-80387 -mno-mmx \
          -mno-red-zone -mcmodel=kernel \
          $(CFLAGS_EXTRA)

LDFLAGS := -nostdlib -static -m elf_x86_64 \
           -z max-page-size=0x1000 -T kernel/linker.ld

KERNEL_SRCS := $(shell find kernel -name '*.c')
KERNEL_OBJS := $(patsubst %.c,build/%.o,$(KERNEL_SRCS))

ASM_SRCS := $(shell find kernel -name '*.asm')
ASM_OBJS := $(patsubst %.asm,build/%.o,$(ASM_SRCS))

all: clean $(KERNEL)

build/%.o: %.c
	@echo "  CC     $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.asm
	@echo "  NASM   $<"
	@mkdir -p $(dir $@)
	@$(NASM) $(NASMFLAGS) $< -o $@

$(KERNEL): $(KERNEL_OBJS) $(ASM_OBJS)
	@mkdir -p $(dir $@)
	@echo "  LD     $@"
	@$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJS) $(ASM_OBJS)
	@printf "\033[32mKernel: $(KERNEL) is ready for use\033[0m\n"

image: $(KERNEL)
	@rm -f $(IMAGE)
	@dd if=/dev/zero of=$(IMAGE) bs=1M count=$(IMAGE_SIZE_MB)
	@mkfs.fat -F 32 $(IMAGE)
	@mmd -i $(IMAGE) ::/EFI
	@mmd -i $(IMAGE) ::/EFI/BOOT
	@mmd -i $(IMAGE) ::/boot
	@mcopy -i $(IMAGE) boot/BOOTX64.EFI ::/EFI/BOOT/
	@mcopy -i $(IMAGE) $(KERNEL) ::/boot/kernel.elf
	@mcopy -i $(IMAGE) boot/limine.conf ::/boot/limine.conf

run: image
	@cp $(OVMF_VARS) build/OVMF_VARS.fd
	@qemu-system-x86_64 \
		-drive if=pflash,format=raw,unit=0,file=$(OVMF_CODE),readonly=on \
		-drive if=pflash,format=raw,unit=1,file=build/OVMF_VARS.fd \
		-drive file=$(IMAGE),format=raw \
		-serial stdio -m 1028M

clean:
	@rm -rf build $(IMAGE)

.PHONY: all image run clean
