#!/bin/bash
# Copyright 2010 Curtis McEnroe <programble@gmail.com>
#
# This file is part of Proxos.
#
# Proxos is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Proxos is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Proxos.  If not, see <http://www.gnu.org/licenses/>.

# Configuration
[ $ASM ] || ASM=nasm
[ $CC ] || CC=clang
[ $LD ] || LD=ld
[ $MKISO ] || MKISO=genisoimage
[ $STAGE2 ] || STAGE2=build/stage2_eltorito
[ $BOOTLOADER ] || BOOTLOADER=grub
[ $ISOLINUXBIN ] || ISOLINUXBIN=/usr/lib/syslinux/isolinux.bin
[ $MBOOTC32 ] || MBOOTC32=/usr/lib/syslinux/mboot.c32

# Ensure the output directory exists
mkdir -p output

# Build functions

function asm
{
    echo " [ ASM ] $1 -> $2"
    $ASM -f elf -o $2 $1
    if [ $? -ne 0 ]; then
        echo " [ ERROR ]"
        exit
    fi
}

function cc
{
    echo " [ CC ] $1 -> $2"
    $CC -m32 -std=c99 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -nostdinc -ffreestanding -fno-builtin -Iinclude/ -o $2 -c $1
    if [ $? -ne 0 ]; then
        echo " [ ERROR ]"
        exit
    fi
}

# Assemble
for FILE in src/*.asm; do
    OUTPUT=output/${FILE##*/}.o
    if [[ $FILE -nt $OUTPUT ]]; then
        asm $FILE $OUTPUT
    fi
done

# Build C sources
for FILE in src/*.c; do
    OUTPUT=output/${FILE##*/}.o
    if [[ $FILE -nt $OUTPUT ]]; then
        cc $FILE $OUTPUT
    fi
done

# Link the kernel
echo " [ LD ] build/linker.ld -> output/kernel.elf"
$LD -melf_i386 -T build/linker.ld -o output/kernel.elf output/*.o
if [ $? -ne 0 ]; then
    echo " [ ERROR ]"
    exit
fi

# Build ISO
mkdir -p output/iso/boot

echo " [ CP ] output/kernel.elf -> output/iso/boot/kernel.elf"
cp output/kernel.elf output/iso/boot/

case "$BOOTLOADER" in
    "grub")
        mkdir -p output/iso/boot/grub
        if [ ! -f output/iso/boot/grub/stage2_eltorito ]; then
            echo " [ CP ] $STAGE2 -> output/iso/boot/grub/stage2_eltorito"
            cp $STAGE2 output/iso/boot/grub/
        fi
        if [[ build/menu.lst -nt output/iso/boot/grub/menu.lst ]]; then
            echo " [ CP ] build/menu.lst -> output/iso/boot/grub/menu.lst"
            cp build/menu.lst output/iso/boot/grub/
        fi
        BOOTFILE=boot/grub/stage2_eltorito
    ;;
    "isolinux")
        mkdir -p output/iso/boot/isolinux
        if [ ! -f output/iso/boot/isolinux/isolinux.bin ]; then
            echo " [ CP ] $ISOLINUXBIN -> output/iso/boot/isolinux/isolinux.bin"
            cp $ISOLINUXBIN output/iso/boot/isolinux/
        fi
        if [ ! -f output/iso/boot/isolinux/mboot.c32 ]; then
            echo " [ CP ] $MBOOTC32 -> output/iso/boot/isolinux/mboot.c32"
            cp $MBOOTC32 output/iso/boot/isolinux/
        fi
        if [[ build/isolinux.cfg -nt output/iso/boot/isolinux/isolinux.cfg ]]; then
            echo " [ CP ] build/isolinux.cfg -> output/iso/boot/isolinux/isolinux.cfg"
            cp build/isolinux.cfg output/iso/boot/isolinux/
        fi
        BOOTFILE=boot/isolinux/isolinux.bin
    ;;
esac


echo " [ ISO ] output/iso/ -> proxos.iso"
$MKISO -R -b $BOOTFILE -no-emul-boot -boot-load-size 4 -boot-info-table -o proxos.iso output/iso/ 2> /dev/null
if [ $? -ne 0 ]; then
    echo " [ ERROR ]"
    exit
fi

# Apply isohybrid to allow for USB booting
if [[ $BOOTLOADER == "isolinux" ]]; then
    echo " [ ISOHYBRID ] proxos.iso -> proxos.iso"
    isohybrid proxos.iso
fi
