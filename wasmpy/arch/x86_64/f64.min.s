.globl _start
_start:
movsd (%rsp), %xmm0
add $8, %rsp
movsd (%rsp), %xmm1
minsd %xmm1, %xmm0
movsd %xmm0, (%rsp)
