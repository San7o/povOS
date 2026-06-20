# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

$(KERNEL_BIN): $(OBJ)
	$(LD) $(KERNEL_LDFLAGS) --oformat binary -o $@ $^
	$(LD) $(KERNEL_LDFLAGS) -o $@.elf $^

$(POVOS_BIN): $(KERNEL_BIN) $(BOOT_BIN)
	cp $(BOOT_BIN) $(POVOS_BIN)
	cat $(KERNEL_BIN) >> $(POVOS_BIN)
	./scripts/check_bootloader_sectors.sh $(ARCH)
	./scripts/patch_size.sh
	dd if=/dev/zero bs=1 count=512 >> $(POVOS_BIN)
	truncate -s %512 $(POVOS_BIN)

$(BOOT_BIN): $(BOOT_ASM)
	$(ASM) -f bin $(BOOT_ASM) -o $@

$(DRIVE).img:
	qemu-img create -f raw $(DRIVE).img $(DRIVE_SIZE)
	echo "TEST_POVOS" | dd of=$(DRIVE).img bs=1 seek=2560 conv=notrunc

%.o: %.c
	@echo " [CC] "$@
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.asm
	@echo " [AS] "$@
	@$(ASM) $(ASFLAGS) $< -o $@

