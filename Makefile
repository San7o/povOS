# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

include config.mk
include $(SUBPROJECTS_MAKE_CONF)

all: povos

##@ Build

povos: $(KERNEL_BIN) $(BOOT_BIN) ## build povOS + bootloader
	cp $(BOOT_BIN) $(POVOS_BIN)
	cat $(KERNEL_BIN) >> $(POVOS_BIN)
	./scripts/check_bootloader_sectors.sh $(ARCH)
	./scripts/patch_size.sh
	dd if=/dev/zero bs=1 count=512 >> $(POVOS_BIN)
	truncate -s %512 $(POVOS_BIN)

$(KERNEL_BIN): $(OBJ)
	$(LD) $(KERNEL_LDFLAGS) --oformat binary -o $@ $^
	$(LD) $(KERNEL_LDFLAGS) -o $@.elf $^

bootloader: $(BOOT_BIN) ## build the bootloader

$(BOOT_BIN): $(BOOT_ASM)
	$(ASM) -f bin $(BOOT_ASM) -o $@

$(DRIVE).img:
	qemu-img create -f raw $(DRIVE).img $(DRIVE_SIZE)
	echo "TEST_POVOS" | dd of=$(DRIVE).img bs=1 seek=2560 conv=notrunc

##@ Run

.PHONY: qemu
qemu: $(DRIVE).img ## run povos with qemu
	truncate -s 10M $(POVOS_BIN)
	$(QEMU) $(QEMU_FLAGS)

.PHONY: bochs
bochs: $(DRIVE).img ## run povos with bochs
	truncate -s 10M $(POVOS_BIN)
	$(BOCHS) -f $(BOCHSRC) -q

.PHONY: debug
debug: ## run povos with a gdb debugging session
	./scripts/debug.sh $(ARCH)

##@ Utils

.PHONY: disas
disas: ## disassemble the bootloader
	ndisasm -b 16 $(BOOT_BIN)

.PHONY: hexdump
hexdump: ## hexdump the bootloader
	hexdump -vC $(POVOS_BIN)

.PHONY: clean
clean: ## clean the build
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(KERNEL_BIN).elf $(POVOS_BIN) $(OBJ)

.PHONY: devices
devices: ## print available qemu devices
	 $(QEMU) -device help

.PHONY: help
help: ## Display this help.
	@awk 'BEGIN {FS = ":.*##"; printf "povOS\n\n    make <target>\n\ntargets:\n"} /^[a-zA-Z_0-9-]+:.*?##/ { printf "  %-15s %s\n", $$1, $$2 } /^##@/ { printf "\n%s\n", substr($$0, 5) } ' ${MAKEFILE_LIST}

%.o: %.c
	@echo " [CC] "$@
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.asm
	@echo " [AS] "$@
	@$(ASM) $(ASFLAGS) $< -o $@

