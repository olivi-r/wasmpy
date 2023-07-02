.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
cmp $0, %eax
je true
bsf %eax, %eax
push %ax
jmp end
true:
pushw $32
end:
pushw $0
pushw $2
