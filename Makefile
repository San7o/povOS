all: boot

.PHONY: boot
boot:
	nasm bootloader/boot.asm -o boot

.PHONY: qemu
qemu:
	qemu-system-x86_64 -drive format=raw,file=boot -display sdl -serial stdio

.PHONY: disas
disas:
	ndisasm -b 16 boot
