.globl _start
_start:
push %ebp
movl %esp, %eax
movl %ebp, %esp
movl %eax, %ebp
sub $0xff00ff, %esp
pop %ax
cmp $4, %ax
je v64
pop %ax
pop %ax
pop %ax
shl $16, %eax
pop %ax
movl %ebp, %esp
pop %ebp
push %ax
shr $16, %eax
push %ax
pushw $2
jmp end
v64:
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
movl %ebp, %esp
pop %ebp
push %cx
shr $16, %ecx
push %cx
push %ax
shr $16, %eax
push %ax
pushw $4
end:
