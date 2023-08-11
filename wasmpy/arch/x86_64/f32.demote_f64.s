.globl _start
_start:
cvtsd2ss (%rsp), %xmm0
add $4, %rsp
movss %xmm0, (%rsp)
