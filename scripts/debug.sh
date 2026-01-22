#!/bin/sh

set -e

POVOS=povos
KERNEL_ELF=kernel/kernel.elf

qemu-system-x86_64 -drive file=$POVOS,format=raw -s -S &
gdb -ex "target remote localhost:1234" -ex "symbol-file $KERNEL_ELF"
