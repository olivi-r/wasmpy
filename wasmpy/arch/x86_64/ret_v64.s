.globl _start
_start:
pop %ax
pop %ax
shl $16, %eax
pop %ax
shl $16, %rax
pop %ax
shl $16, %rax
pop %ax
movq %rax, (0xff00000000000000)
movq %rdx, %rax
movq $0xff000000000000ff, %rax
movq %rbp, %rsp
pop %rbp
ret
