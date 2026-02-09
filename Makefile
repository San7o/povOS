# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

ARCH ?= x86_64

CC       = clang
LD      ?= ld.lld-19
AR       = llvm-ar
ASM     ?= nasm
QEMU    ?= qemu-system-$(ARCH)
BOCHS   ?= bochs
BOCHSRC ?= scripts/bochsrc.txt
GDB     ?= gdb

OBJ     =

SUBPROJECTS_MAKE_CONF = kernel/make.conf        \
                        kernel/input/make.conf  \
                        kernel/mm/make.conf     \
                        libk/make.conf          \
                        arch/$(ARCH)/make.conf  \
                        drivers/make.conf       \
                        drivers/video/make.conf \
                        drivers/input/make.conf

include $(SUBPROJECTS_MAKE_CONF)

CFLAGS  = --target=$(ARCH)-elf \
          -std=c99 \
          -ggdb \
          -c \
          -ffreestanding \
          --target=x86_64-none-elf \
				  -fno-stack-protector \
          -fno-pic \
          -fno-pie \
          -mno-red-zone \
          -m64 \
          -nostdlib \
          -nodefaultlibs \
          -fno-builtin \
          -Wall -Wextra -Werror \
          -I include
KERNEL_LDFLAGS  = -T kernel/linker.ld
ASFLAGS         = -f elf64
BOOT_BIN    = bootloader/$(ARCH)/boot
BOOT_ASM    = bootloader/$(ARCH)/boot.asm
KERNEL_BIN  = kernel/kernel
POVOS_BIN   = povos

#
# Targets
#

all: povos

povos: $(KERNEL_BIN) $(BOOT_BIN)
	cp $(BOOT_BIN) $(POVOS_BIN)
	cat $(KERNEL_BIN) >> $(POVOS_BIN)
	./scripts/check_bootloader_sectors.sh $(ARCH)
	./scripts/patch_size.sh
	dd if=/dev/zero bs=1 count=512 >> $(POVOS_BIN)
	truncate -s %512 $(POVOS_BIN)

$(KERNEL_BIN): $(OBJ)
	$(LD) $(KERNEL_LDFLAGS) --oformat binary -o $@ $^
	$(LD) $(KERNEL_LDFLAGS) -o $@.elf $^

$(BOOT_BIN): $(BOOT_ASM)
	$(ASM) -f bin $(BOOT_ASM) -o $@

.PHONY: qemu
qemu:
	$(QEMU) -drive format=raw,file=$(POVOS_BIN) -display sdl -serial stdio

.PHONY: bochs
bochs:
	$(BOCHS) -f $(BOCHSRC) -q

.PHONY: disas
disas:
	ndisasm -b 16 $(BOOT_BIN)

.PHONY: hexdump
hexdump:
	hexdump -vC $(POVOS_BIN)

.PHONY: debug
debug:
	./scripts/debug.sh $(ARCH)

.PHONY: clean
clean:
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(KERNEL_BIN).elf $(POVOS_BIN) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.asm
	$(ASM) $(ASFLAGS) $< -o $@
