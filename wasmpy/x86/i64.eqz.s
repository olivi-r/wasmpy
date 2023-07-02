.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
cmp %ecx, %eax
jne false
cmp $0, %eax
jne false
pushw $1
jmp end
false:
pushw $0
end:
pushw $0
pushw $2
