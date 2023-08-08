.globl _start
_start:
movss (%rsp), %xmm0
addq $4, %rsp
movss (%rsp), %xmm1
minss %xmm1, %xmm0
movss %xmm0, (%rsp)
