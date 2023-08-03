.globl _start
_start:
pop %ax
cmp $4, %ax
je v64
pop %ax
shl $16, %eax
pop %ax
movl %eax, %ecx
push %cx
shr $16, %ecx
push %cx
pushw $2
push %ebp
movl %esp, %ecx
movl %ebp, %esp
movl %ecx, %ebp
sub $0xff00ff, %esp
push %ax
shr $16, %eax
push %ax
pushw $2
movl %ebp, %esp
pop %ebp
jmp end
v64:
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
movl %eax, %edx
movl %ecx, %ebx
push %bx
shr $16, %ebx
push %bx
push %dx
shr $16, %edx
push %dx
pushw $4
push %ebp
movl %esp, %edx
movl %ebp, %esp
movl %edx, %ebp
sub $0xff00ff, %esp
push %cx
shr $16, %ecx
push %cx
push %ax
shr $16, %eax
push %ax
pushw $4
movl %ebp, %esp
pop %ebp
end:
