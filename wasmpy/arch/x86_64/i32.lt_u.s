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
cmp %eax, %ecx
jb true
pushw $0
jmp end
true:
pushw $1
end:
pushw $0
pushw $2
