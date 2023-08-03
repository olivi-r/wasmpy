.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
cmp $0, %eax
je true
pushw $0
jmp end
true:
pushw $1
end:
pushw $0
pushw $2
