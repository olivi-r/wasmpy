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
movq %rbp, %rsp
pop %rbp
ret
nezero:
cmp $0x80000000, %eax
jne cont
cmp $0xFFFFFFFF, %ecx
jne cont
movq $0xff000000000000ff, %rax
movq %rbp, %rsp
pop %rbp
ret
cont:
movl $0, %edx
idiv %ecx
push %ax
shr $16, %eax
push %ax
pushw $2
