.globl _start
_start:
movw $32, %cx
pop %dx
pop %dx
pop %ax
idiv %cx
pop %ax
pop %ax
shl $16, %eax
pop %ax
movb %dl, %cl
shl %cl, %eax
push %ax
shr $16, %eax
push %ax
pushw $2
