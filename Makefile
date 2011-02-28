# ISO Options
STAGE2=/usr/lib/grub/i386-pc/stage2_eltorito
GENISOIMAGE=genisoimage
ISO=proxos.iso

# Compiling options
CC=clang
ASM=nasm
LD=ld

CINCLUDES=-Iinclude/
CWARNINGS=-Werror -Wall -Wextra -Wunreachable-code -Wcast-qual -Wcast-align -Wswitch-enum -Wmissing-noreturn -Wwrite-strings -Wundef -Wpacked -Wredundant-decls -Winline -Wdisabled-optimization -Wbad-function-cast
CFLAGS=-m32 -std=c99 -nostdinc -ffreestanding -fno-builtin
DFLAGS=-ggdb -DDEBUG -O0

AFLAGS=-f elf

LDFLAGS=-melf_i386 -nostdlib -T linker.ld

# Sources
CSOURCES:=$(wildcard src/*.c)
COBJECTS:=$(CSOURCES:%.c=%.o)

ASOURCES:=$(wildcard src/*.asm)
AOBJECTS:=$(ASOURCES:%.asm=%.ao)

KERNEL=proxos.elf

all: Makefile iso

iso: $(ISO)
$(ISO): iso/boot/grub/stage2_eltorito iso/boot/grub/menu.lst iso/boot/$(KERNEL)
	$(GENISOIMAGE) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o $(ISO) iso/

iso/boot/grub/stage2_eltorito: $(STAGE2)
	mkdir -p iso/boot/grub/
	cp $< $@

iso/boot/$(KERNEL): $(KERNEL)
	mkdir -p iso/boot/
	cp $< $@

iso/boot/grub/menu.lst: menu.lst
	mkdir -p iso/boot/grub/
	cp $< $@
	echo "kernel /boot/$(KERNEL)" >> $@

qemu: iso
	qemu -serial stdio -soundhw pcspk -cdrom $(ISO)

qemu-gdb: iso
	qemu -serial stdio -soundhw pcspk -s -S -cdrom $(ISO)

bochs: iso
	bochs -q

kernel: $(KERNEL)

$(KERNEL): include/version.h $(AOBJECTS) $(COBJECTS)
	$(LD) $(LDFLAGS) $(AOBJECTS) $(COBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CWARNINGS) $(CINCLUDES) -c $< -o $@

%.ao: %.asm
	$(ASM) $(AFLAGS) $< -o $@

include/version.h: $(CSOURCES)
	echo "#define VERSION \"$(shell git describe --tags HEAD | cut -c2-)\"" > $@
	echo "#define COMPILER \"$(CC) \" __VERSION__" >> $@

debug:
	@$(MAKE) $(MFLAGS) $(KERNEL) CFLAGS="$(CFLAGS) $(DFLAGS)"

dry-asm:
	@$(MAKE) $(MFLAGS) CFLAGS="$(CFLAGS) -S" $(COBJECTS)

dry-pp:
	@$(MAKE) $(MFLAGS) CFLAGS="$(CFLAGS) -E" $(COBJECTS)

clean:
	rm -f include/version.h
	rm -f $(COBJECTS)
	rm -f $(AOBJECTS)
	rm -f $(KERNEL)
	rm -f $(ISO)
	rm -rf iso/

todo:
	@grep -rInso 'TODO: \([^*]\+\)' src/ include/

sloc:
	@sloccount ./ | grep "(SLOC)"

check-syntax: include/version.h
	$(CC) $(CFLAGS) $(CWARNINGS) $(CINCLUDES) -fsyntax-only $(CHK_SOURCES)

.PHONY: iso clean qemu qemu-gdb todo sloc check-syntax debug dry-pp dry-asm

