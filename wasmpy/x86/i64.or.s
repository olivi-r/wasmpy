.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
pop %dx
pop %dx
shl $16, %edx
pop %dx
pop %bx
shl $16, %ebx
pop %bx
or %edx, %eax
or %ebx, %ecx
push %cx
shr $16, %ecx
push %cx
push %ax
shr $16, %eax
push %ax
pushw $4
