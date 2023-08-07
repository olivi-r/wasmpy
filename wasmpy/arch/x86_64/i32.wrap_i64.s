.globl _start
_start:
popq %rax
subq $4, %rsp
movl %eax, (%rsp)
