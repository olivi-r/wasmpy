.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
cmp $0, %eax
je true
bsr %eax, %eax
movw $31, %cx
sub %ax, %cx
push %cx
jmp end
true:
pushw $32
end:
pushw $0
pushw $2
