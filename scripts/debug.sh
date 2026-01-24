#!/bin/sh

set -e

if [ $# -ne 1 ]; then
    echo Please specify the arch in the first argument
    exit 1
fi

ARCH=$1

POVOS=povos
KERNEL_ELF=kernel/kernel.elf

qemu-system-$ARCH -drive file=$POVOS,format=raw -s -S &
gdb -ex "target remote localhost:1234" -ex "symbol-file $KERNEL_ELF"
