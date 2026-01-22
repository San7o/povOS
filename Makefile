CC    = clang
LD   ?= ld.lld-19
AR    = llvm-ar
ASM  ?= nasm
QEMU ?= qemu-system-x86_64
GDB  ?= gdb

OBJ     = kernel/main.o kernel/entry.o
CFLAGS  = --target=x86_64-elf \
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
          -Wall -Wextra -Werror
KERNEL_LDFLAGS  = -T kernel/linker.ld
ASFLAGS         = -f elf64
BOOT_BIN    = bootloader/boot
KERNEL_BIN  = kernel/kernel
POVOS_BIN   = povos

#
# Targets
#

all: povos

povos: $(KERNEL_BIN) $(BOOT_BIN)
	cp $(BOOT_BIN) $(POVOS_BIN)
	cat $(KERNEL_BIN) >> $(POVOS_BIN)
	./scripts/check_bootloader_sectors.sh
	./scripts/patch_size.sh
	dd if=/dev/zero bs=1 count=512 >> $(POVOS_BIN)

$(KERNEL_BIN): $(OBJ)
	$(LD) $(KERNEL_LDFLAGS) --oformat binary -o $@ $^
	$(LD) $(KERNEL_LDFLAGS) -o $@.elf $^

$(BOOT_BIN): bootloader/boot.asm
	$(ASM) -f bin bootloader/boot.asm -o $@

.PHONY: qemu
qemu:
	$(QEMU) -drive format=raw,file=$(POVOS_BIN) -display sdl -serial stdio

.PHONY: disas
disas:
	ndisasm -b 16 $(BOOT_BIN)

.PHONY: hexdump
hexdump:
	hexdump -vC $(POVOS_BIN)

.PHONY: debug
debug:
	./scripts/debug.sh

.PHONY: clean
clean:
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(KERNEL_BIN).elf $(POVOS_BIN) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.asm
	$(ASM) $(ASFLAGS) $< -o $@
