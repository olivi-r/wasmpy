.globl _start
_start:
pop %cx
pop %cx
shl $16, %ecx
pop %cx
pop %ax
pop %ax
shl $16, %eax
pop %ax
rol %cl, %eax
push %ax
shr $16, %eax
push %ax
pushw $2
