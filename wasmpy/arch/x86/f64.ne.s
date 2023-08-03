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
cmp %edx, %eax
jne true
cmp %ebx, %ecx
jne true
pushw $0
jmp end
true:
pushw $1
end:
pushw $0
pushw $2
