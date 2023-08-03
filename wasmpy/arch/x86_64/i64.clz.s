.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
cmp $0, %eax
je highzero
bsr %eax, %eax
movw $31, %cx
sub %ax, %cx
push %cx
jmp end
highzero:
cmp $0, %ecx
je lowzero
bsr %ecx, %ecx
movw $63, %ax
sub %cx, %ax
push %ax
jmp end
lowzero:
pushw $64
end:
pushw $0
pushw $0
pushw $0
pushw $4
