.globl _start
_start:
push %ax
shr $16, %eax
push %ax
pushw $2
pushw $0
pushw $0
