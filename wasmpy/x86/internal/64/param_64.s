.globl _start
_start:
push %ax
shr $16, %rax
push %ax
shr $16, %rax
push %ax
shr $16, %eax
push %ax
pushw $4
