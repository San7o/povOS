all: boot

.PHONY: boot
boot:
	nasm bootloader/boot.asm

.PHONY: qemu
qemu:
	qemu-system-x86_64 -drive format=raw,file=boot -display sdl -serial stdio
