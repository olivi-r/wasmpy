.globl _start
_start:
movl (%rsp), %eax
cvtsi2ssq %rax, %xmm0
movss %xmm0, (%rsp)
