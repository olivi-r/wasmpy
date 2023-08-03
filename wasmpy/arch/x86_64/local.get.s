.globl _start
_start:
push %rbp
movq %rsp, %rax
movq %rbp, %rsp
movq %rax, %rbp
sub $0xff00ff, %rsp
pop %ax
cmp $4, %ax
je v64
pop %ax
pop %ax
pop %ax
shl $16, %eax
pop %ax
movq %rbp, %rsp
pop %rbp
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
movq %rbp, %rsp
pop %rbp
push %cx
shr $16, %ecx
push %cx
push %ax
shr $16, %eax
push %ax
pushw $4
end:
