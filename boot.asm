; boot.s - Cabeçalho Multiboot1 para GRUB
MBALIGN  equ  1 << 0                   ; Alinhamento de módulos
MEMINFO  equ  1 << 1                   ; Tabela de memória
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002               ; Magic number do Multiboot
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KB de pilha (stack)
stack_top:

section .text
global _start:
extern kernel_main

_start:
    mov esp, stack_top
    call kernel_main

    cli
.hang:
    hlt
    jmp .hang