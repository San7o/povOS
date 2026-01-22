#!/bin/sh

set -e

EXPECTED_SECTORS=4
BOOTLOADER=bootloader/boot
BOOTLOADER_SIZE=$(wc -c < $BOOTLOADER)
BOOTLOADER_SECTORS=$(( ($BOOTLOADER_SIZE + 511) / 512 ))

echo Bootloader is size $BOOTLOADER_SIZE, sectors $BOOTLOADER_SECTORS

# The start address of the kernel is determined by the equation
# 0x7C00 + 512 * $BOOTLOADER_SECTORS
# If the size of the bootloader changes, then we need to change
# the address of the kernel in the bootloader and kernel linker

if [ $BOOTLOADER_SECTORS -ne $EXPECTED_SECTORS ]; then
    echo Expected $EXPECTED_SECTORS sectors in bootloader, got $BOOTLOADER_SECTORS
    exit 1
fi
