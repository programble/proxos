global loader
global Kernel_stackPointer
extern Kernel_main ; main.c

; Multiboot
MODULEALIGN equ 1<<0
MEMINFO equ 1<<1
FLAGS equ MODULEALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .mbheader
align 4
MultiBootHeader:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text

STACKSIZE equ 0x4000

loader:
    mov esp, stack+STACKSIZE ; Stack setup
    push eax ; Multiboot magic
    push ebx ; Miltiboot info

    call Kernel_main ; Call C kernel

    cli

hang:
    hlt
    jmp hang

section .bss
align 4
stack:
    resb STACKSIZE
Kernel_stackPointer:
