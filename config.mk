# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

#
# Configuration flags
#

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

SUBPROJECTS_MAKE_CONF = \
    kernel/make.conf        \
    mm/make.conf            \
    fs/make.conf            \
    libk/make.conf          \
    arch/$(ARCH)/make.conf  \
    drivers/make.conf

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
          -mno-sse -mno-sse2 \
          -m64 \
          -mcmodel=kernel \
          -nostdlib \
          -nodefaultlibs \
          -fno-builtin \
          -Wall -Wextra -Werror -Wpedantic \
          -Wno-unused-function \
          -Wno-empty-translation-unit \
          -I include -I arch/${ARCH}/include
KERNEL_LDFLAGS  = -T kernel/linker.ld
ASFLAGS         = -f elf64
BOOT_BIN    = bootloader/$(ARCH)/boot
BOOT_ASM    = bootloader/$(ARCH)/boot.asm
KERNEL_BIN  = kernel/kernel
POVOS_BIN   = povos
DRIVE       = sample_drive
DRIVE_SIZE  = 10M

# + PCIe
# - ATA
QEMU_MACHINE ?= pc-q35-9.0
# - PCIe
# + ATA
#QEMU_MACHINE ?= pc

QEMU_CPU  ?= qemu64
# +random number generation
#QEMU_CPU ?= Skylake-Client

QEMU_FLAGS  = -M $(QEMU_MACHINE) \
              -cpu $(QEMU_CPU) \
              -drive format=raw,file=$(POVOS_BIN),if=ide \
              -display sdl \
              -serial stdio \
              -device edu \
              -drive file=$(DRIVE).img,if=none,id=$(DRIVE),format=raw \
              -device ide-hd,drive=$(DRIVE),bus=ide.1,unit=0
