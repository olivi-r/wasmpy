.globl _start
_start:
pop %cx
pop %cx
shl $16, %ecx
pop %cx
pop %ax
pop %ax
shl $16, %eax
pop %ax
cmp $0, %ecx
jne nezero
movq $0xff00000000000000, %rax
mov %rbp, %rsp
pop %rbp
ret
nezero:
movl $0, %edx
idiv %ecx
push %ax
shr $16, %eax
push %ax
pushw $2
