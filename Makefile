# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

include config.mk
include $(SUBPROJECTS_MAKE_CONF)

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

bootloader: $(BOOT_BIN)

$(BOOT_BIN): $(BOOT_ASM)
	$(ASM) -f bin $(BOOT_ASM) -o $@

$(DRIVE).img:
	qemu-img create -f raw $(DRIVE).img $(DRIVE_SIZE)
	echo "TEST_POVOS" | dd of=$(DRIVE).img bs=1 seek=2560 conv=notrunc

.PHONY: qemu
qemu: $(DRIVE).img
	truncate -s 10M $(POVOS_BIN)
	$(QEMU) $(QEMU_FLAGS)

.PHONY: bochs
bochs: $(DRIVE).img
	truncate -s 10M $(POVOS_BIN)
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

.PHONY: devices
devices:
	 $(QEMU) -device help

%.o: %.c
	@echo " [CC] "$@
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.asm
	@echo " [AS] "$@
	@$(ASM) $(ASFLAGS) $< -o $@
