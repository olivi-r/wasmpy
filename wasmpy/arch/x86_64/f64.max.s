.globl _start
_start:
movsd (%rsp), %xmm0
addq $8, %rsp
movsd (%rsp), %xmm1
maxsd %xmm1, %xmm0
movsd %xmm0, (%rsp)