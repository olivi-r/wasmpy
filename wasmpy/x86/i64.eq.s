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
jne false
cmp %ebx, %ecx
jne false
pushw $1
jmp end
false:
pushw $0
end:
pushw $0
pushw $2
