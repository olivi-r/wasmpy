.globl _start
_start:
push %ax
shr $16, %eax
push %ax
push %cx
shr $16, %ecx
push %cx
pushw $4
