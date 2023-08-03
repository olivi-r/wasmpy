.globl _start
_start:
push %dx
shr $16, %edx
push %dx
pushw $2
pushw $0
pushw $0
