#!/bin/sh

# Use this script to create a live usb and boot the OS on real hardware

# Set this accordin to your device
DEV=/dev/sda

dd if=povos of=$DEV bs=4M status=progress conv=fsync
sync
