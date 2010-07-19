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
