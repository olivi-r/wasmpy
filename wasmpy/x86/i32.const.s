.globl _start
_start:
movw $0x0000, %ax
push %ax
movw $0xffff, %ax
push %ax
pushw $2
