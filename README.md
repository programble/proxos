Proxos
======
![](http://stillmaintained.com/programble/proxos.png)

Programble's toy OS.

## Building

    make

### Dependencies

  * GRUB (Or at least `stage2_eltorito`, which can be downloaded [here](http://misc.duckinator.net/stage2_eltorito))
  * `genisoimage` or `mkiso`
  * C99 Compiler
  * NASM
  * Linker

### Options

  * `STAGE2` Default: `/usr/lib/grub/i386-pc/stage2_eltorito`
  * `GENISOIMAGE` Default: `genisoimage`
  * `CC` Default: `clang`
  * `ASM` Default: `nasm`
  * `LD` Default: `ld`
  * `KERNEL` Default: `proxos.elf`
  * `ISO` Default: `proxos.iso`

### Tested Compilers

  * Clang on Linux x86_64
  * GCC on Linux x86_64

## Running

Two makefile targets are provided for starting Proxos in a VM:

    make qemu
    make bochs

## Features

Things I can brag about:

  * A nice memory manager
  * A PC Speaker Driver (Woooo!)
  * Threading
  * A re-usable locking system (No global kernel lock)

## License

Copyright (c) 2011, Curtis McEnroe <programble@gmail.com>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
