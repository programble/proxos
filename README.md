Proxos
======

Programble's attempt at an OS.

Building
--------

To build Proxos, you will need:

 * NASM (or other compatible assembler)
 * clang (or other compatible C compiler, such as GCC)
 * ld (or other compatible linker)
 * mkiso/genisoimage
 * BASH

To build Proxos with default settings, run

    ./build.sh

To build Proxos using GCC, run

    CC=gcc ./build.sh

### Building Proxos for USB

To build Proxos for USB, you will also need:

 * Syslinux
 * Isolinux

The default settings assume Syslinux is installed in
`/usr/lib/syslinux`.

To build Proxos for USB, run

    BOOTLOADER=isolinux ./build.sh

The output ISO will automatically be patched with the `isohybrid`
command, allowing the ISO to be booted from a USB stick.

To write the ISO to a USB stick, run (as root)

    dd if=proxos.iso of=/dev/sdx

Where `x` is the corresponding letter of the USB stick.
    
### Other build settings

 * `ASM` Name of assembler to use (default: `nasm`)
 * `CC` Name of C compiler to use (default: `clang`)
 * `LD` Name of linker to use (default: `ld`)
 * `MKISO` Name of `mkiso` or `genisoimage` (default: `genisoimage`)
 * `STAGE2` Path to GRUB `stage2_eltorito` file (default: `build/stage2_eltorito`)
 * `BOOTLOADER` Which bootloader to use (one of either `grub` or `isolinux`) (default: `grub`)
 * `ISOLINUXBIN` The path to `isolinux.bin` (default: `/usr/lib/syslinux/isolinux.bin`)
 * `MBOOTC32` The path to `mboot.c32` multiboot module for Syslinux (default: `/usr/lib/syslinux/mboot.c32`)
