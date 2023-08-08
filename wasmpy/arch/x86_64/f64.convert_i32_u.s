.globl _start
_start:
movl (%rsp), %eax
cvtsi2sdq %rax, %xmm0
subq $4, %rsp
movsd %xmm0, (%rsp)
