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

### Other build settings

 * `ASM` Name of assembler to use (default: `nasm`)
 * `CC` Name of C compiler to use (default: `clang`)
 * `LD` Name of linker to use (default: `ld`)
 * `MKISO` Name of `mkiso` or `genisoimage` (default: `genisoimage`)
 * `STAGE2` Path to GRUB `stage2_eltorito` file (default: `build/stage2_eltorito`)
 
