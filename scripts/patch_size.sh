#!/bin/sh

set -e

KERNEL=kernel/kernel
OS=povos

KERNEL_SIZE=$(wc -c < $KERNEL)
KERNEL_SECTORS=$(( ($KERNEL_SIZE + 511) / 512 ))
printf %02x $KERNEL_SECTORS | xxd -r -p | dd of=$OS bs=1 seek=2 count=1 conv=notrunc

echo Patched kernel with size $KERNEL_SIZE, sectors $KERNEL_SECTORS
