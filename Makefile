# SPDX-License-Identifier: MIT
# Author:  Giovanni Santini
# Mail:    giovanni.santini@proton.me
# Github:  @San7o

all: povos

include config.mk
include $(SUBPROJECTS_MAKE_CONF)
include rules.mk

##@ Build

.PHONY: povos
povos: $(POVOS_BIN) ## build povOS + bootloader

.PHONY: bootloader
bootloader: $(BOOT_BIN) ## build the bootloader

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
