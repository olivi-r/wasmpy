.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
pop %cx
pop %cx
shl $16, %ecx
pop %cx
imul %ecx, %eax
push %ax
shr $16, %eax
push %ax
pushw $2
