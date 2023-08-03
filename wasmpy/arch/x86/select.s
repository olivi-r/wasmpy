.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
cmp $0, %eax
je zero
pop %ax
cmp $2, %ax
je nezerox32
pop %ax
pop %ax
nezerox32:
pop %ax
pop %ax
jmp end
zero:
pop %ax
cmp $2, %ax
je zerox32
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
pop %dx
pop %dx
pop %dx
pop %dx
pop %dx
push %cx
shr $16, %ecx
push %cx
push %ax
shr $16, %eax
push %ax
pushw $4
jmp end
zerox32:
pop %ax
shl $16, %eax
pop %ax
pop %cx
pop %cx
pop %cx
push %ax
shr $16, %eax
push %ax
pushw $2
end:
