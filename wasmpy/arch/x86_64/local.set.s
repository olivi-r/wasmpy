.globl _start
_start:
pop %ax
cmp $4, %ax
je v64
pop %ax
shl $16, %eax
pop %ax
push %rbp
movq %rsp, %rcx
movq %rbp, %rsp
movq %rcx, %rbp
sub $0xff00ff, %rsp
push %ax
shr $16, %eax
push %ax
pushw $2
movq %rbp, %rsp
pop %rbp
jmp end
v64:
pop %ax
shl $16, %eax
pop %ax
pop %cx
shl $16, %ecx
pop %cx
push %rbp
movq %rsp, %rdx
movq %rbp, %rsp
movq %rdx, %rbp
sub $0xff00ff, %rsp
push %cx
shr $16, %ecx
push %cx
push %ax
shr $16, %eax
push %ax
pushw $4
movq %rbp, %rsp
pop %rbp
end:
